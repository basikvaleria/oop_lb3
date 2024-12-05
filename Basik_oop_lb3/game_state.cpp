#include "game_state.h"
#include "md5.h"

GameState::GameState(
    shared_ptr<Battleground> player_field, 
    shared_ptr<Battleground> enemy_field,
    shared_ptr<ShipsManager> player_ships, 
    shared_ptr<ShipsManager> enemy_ships
) : 
    m_player_field(player_field), 
    m_enemy_field(enemy_field), 
    m_player_ships(player_ships),
    m_enemy_ships(enemy_ships), 
    m_info_holder(enemy_field, enemy_ships, 0, 0), 
    m_ability_manager(AbilityManager()) {}

vector<pair<size_t, size_t>> GameState::getShipPosition(Ship& ship, Battleground& field) const {
    vector<pair<size_t, size_t>> res;
    for (size_t x = 0; x < field.getWidth(); ++x){
        for (size_t y = 0; y < field.getHeight(); ++y){
            if (!field.getCell(x, y).isEmpty() && &field.getCell(x, y).getSegment() == &ship.getSegment(0)){
                for (size_t i = 0; i < ship.len(); ++i){
                    if (ship.getOrientation() == Ship::Orientation::VERTICAL)
                        res.push_back(make_pair(x, y + i));
                    else
                        res.push_back(make_pair(x + i, y));
                }
            }
        }
    }
    return res;
}

string GameState::serializeShips(ShipsManager& ships_manager, Battleground& field) const {
    string s = to_string(ships_manager.len()) + "\n";
    for (size_t i = 0; i < ships_manager.len(); ++i) {
        s += to_string(ships_manager.getShip(i).len()) + " ";
        s += to_string(static_cast<int>(ships_manager.getShip(i).getOrientation())) + "|";
        auto coords = this->getShipPosition(ships_manager.getShip(i), field);
        for (const auto& coord : coords)
            s += to_string(coord.first) + "," + to_string(coord.second) + "," + to_string(static_cast<int>(ships_manager.getShip(i).getSegment(0).getState())) + " ";
        s += "\n";
    }
    return s;
}

string GameState::serializeField(Battleground& field) const {
    string s = to_string(field.getWidth()) + " " + to_string(field.getHeight()) + "\n";
    for (size_t y = 0; y < field.getHeight(); ++y) {
        for (size_t x = 0; x < field.getWidth(); ++x) {
            s += to_string(static_cast<int>(field.getCell(x, y).getState())) + " ";
        }
        s += "\n";
    }
    return s;
}

string GameState::serializeSkills(const AbilityManager& ability_manager) const {
    string s = to_string(ability_manager.len()) + "\n";
    for (const auto& skill : ability_manager.getSkills()) {
        if (dynamic_cast<DoubleDamage*>(skill.get()))
            s += "DoubleDamage\n";
        else if (dynamic_cast<RandomAttack*>(skill.get()))
            s += "RandomAttack\n";
        else if (dynamic_cast<Scanner*>(skill.get()))
            s += "Scanner\n";
    }
    return s;
}

ostream& operator<<(ostream& os, const GameState& game_state) {
    string s = game_state.serializeShips(*game_state.m_player_ships, *game_state.m_player_field);
    s += game_state.serializeShips(*game_state.m_enemy_ships, *game_state.m_enemy_field);
    s += game_state.serializeField(*game_state.m_player_field);
    s += game_state.serializeField(*game_state.m_enemy_field);
    s += game_state.serializeSkills(game_state.m_ability_manager);
    s += to_string(game_state.m_info_holder.x) + " " + to_string(game_state.m_info_holder.y) + "\n";
    s += to_string(game_state.m_is_player_turn) + "\n";
    s += to_string(game_state.m_is_game_started) + "\n";
    os << s << md5(s);
    return os;
}

vector<string> GameState::split(const string& s, char delim) {
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> GameState::readShips(vector<string>& lines, size_t& j) {
    vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> ships;
    for (size_t i = j; i < stoi(lines[j - 1]) + j; ++i) {
        auto s = split(lines[i], '|');
        size_t len = stoi(split(s[0], ' ')[0]);
        Ship::Orientation orientation = static_cast<Ship::Orientation>(stoi(split(s[0], ' ')[1]));
        vector<tuple<size_t, size_t, Segment::State>> segments;
        if (s.size() == 2) {
            auto segments_str = split(s[1], ' ');
            for (const auto& segment_str : segments_str) {
                auto data = split(segment_str, ',');
                segments.emplace_back(stoi(data[0]), stoi(data[1]), static_cast<Segment::State>(stoi(data[2])));
            }
        }
        ships.push_back(make_tuple(len, orientation, segments));
    }
    j += stoi(lines[j - 1]) + 1;
    return ships;
}

void GameState::updateFieldWithShips(Battleground& field, ShipsManager& ships_manager, vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>>& ships, vector<string>& lines, size_t& j) {
    for (size_t k = 0; k < ships.size(); ++k) {
        auto ship = ships[k];
        Ship::Orientation orientation = get<1>(ship);
        auto segments = get<2>(ship);
        if (!segments.empty()) {
            field.addShip(get<0>(segments[0]), get<1>(segments[0]), ships_manager.getShip(k), orientation);
            for (const auto& segment : segments)
                field.getCell(get<0>(segment), get<1>(segment)).getSegment().setState(get<2>(segment));
        }
    }
    for (size_t y = 0; y < field.getHeight(); ++y) {
        auto states = split(lines[j + y], ' ');
        for (size_t x = 0; x < field.getWidth(); ++x) {
            field.getCell(x, y).setState(static_cast<Cell::ViewState>(stoi(states[x])));
        }
    }
    j += stoi(lines[j - 1]);
    j++;
}

istream& operator>>(istream& is, GameState& game_state) {
    stringstream buffer;
    buffer << is.rdbuf();
    auto line = buffer.str();
    auto lines = game_state.split(line, '\n');

    if (lines.size() < 2)
        throw invalid_argument("Incorrect save file");

    string save;
    for (size_t i = 0; i < lines.size() - 1; ++i)
        save += lines[i] + "\n";

    if (md5(save) != lines.back())
        throw invalid_argument("Save file has been modified. Its not allowed!");

    size_t j = 1;

    auto player_ships = game_state.readShips(lines, j);
    auto enemy_ships = game_state.readShips(lines, j);

    vector<size_t> player_ship_lens;
    vector<size_t> enemy_ship_lens;
    for (const auto& ship : player_ships)
        player_ship_lens.push_back(get<0>(ship));
    for (const auto& ship : enemy_ships)
        enemy_ship_lens.push_back(get<0>(ship));

    game_state.m_player_ships = make_shared<ShipsManager>(ShipsManager(player_ship_lens.size(), player_ship_lens));
    game_state.m_enemy_ships = make_shared<ShipsManager>(ShipsManager(enemy_ship_lens.size(), enemy_ship_lens));

    auto s = game_state.split(lines[j - 1], ' ');
    game_state.m_player_field = make_shared<Battleground>(Battleground(stoi(s[0]), stoi(s[1])));
    game_state.updateFieldWithShips(*game_state.m_player_field, *game_state.m_player_ships, player_ships, lines, j);

    s = game_state.split(lines[j - 1], ' ');
    game_state.m_enemy_field = make_shared<Battleground>(Battleground(stoi(s[0]), stoi(s[1])));
    game_state.updateFieldWithShips(*game_state.m_enemy_field, *game_state.m_enemy_ships, enemy_ships, lines, j);

    game_state.m_player_ships->getNewDeads();
    game_state.m_enemy_ships->getNewDeads();

    size_t skills_count = stoi(lines[j - 1]);
    game_state.m_ability_manager = AbilityManager();
    size_t base_skills_count = game_state.m_ability_manager.len();
    for (size_t i = 0; i < base_skills_count; ++i)
        game_state.m_ability_manager.getAbility();

    vector<size_t> skills;
    for (size_t i = 0; i < skills_count; ++i) {
        if (lines[i + j] == "Scanner") {
            skills.push_back(2);
        } else if (lines[i + j] == "RandomAttack") {
            skills.push_back(1);
        } else if (lines[i + j] == "DoubleDamage") {
            skills.push_back(0);
        }
    }

    for (const auto& skill : skills) {
        game_state.m_ability_manager.addAbility(skill);
    }

    j += skills_count + 1;

    s = game_state.split(lines[j - 1], ' ');
    game_state.m_info_holder.x = static_cast<size_t>(stoi(s[0]));
    game_state.m_info_holder.y = static_cast<size_t>(stoi(s[1]));
    game_state.m_info_holder.battleground = game_state.m_enemy_field;
    game_state.m_info_holder.ships_manager = game_state.m_enemy_ships;

    game_state.m_is_game_started = static_cast<bool>(stoi(lines[j]));
    game_state.m_is_player_turn = static_cast<bool>(stoi(lines[j + 1]));

    return is;
}
