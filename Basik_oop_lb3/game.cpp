#include "game.h"

Game::Game(
    shared_ptr<Battleground> player_field, shared_ptr<Battleground> enemy_field,
    shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships
):  game_state(player_field, enemy_field, player_ships, enemy_ships) {}

void Game::start() {
    this->game_state.m_is_game_started = true;
    this->game_state.m_is_player_turn = true;
}

void Game::playerTurn(size_t x, size_t y, bool use_skill, size_t skill_x, size_t skill_y) {
    this->check_game_status();
    if (use_skill) {
        this->game_state.m_info_holder.x = skill_x;
        this->game_state.m_info_holder.y = skill_y;
        try{
            this->game_state.m_ability_manager.getAbility()->applyAbility(this->game_state.m_info_holder);
        }
        catch(NoAbilityException& e){
            cerr<<e.what()<<endl;
        }
    }
    try{
        this->game_state.m_enemy_field->attack(x, y);
    }
    catch(CoordinatesException& e){
        cerr<<e.what()<<endl;
    }
    this->game_state.m_ability_manager.addRandomAbilites(this->game_state.m_enemy_ships->getNewDeads());
    this->game_state.m_is_player_turn = false;
}

void Game::enemyTurn() {
    random_device rd;
    default_random_engine engine(rd());
    this->check_game_status(true);
    size_t x = engine() % this->game_state.m_player_field->getWidth();
    size_t y = engine() % this->game_state.m_player_field->getHeight();
    this->game_state.m_player_field->attack(x, y);
    this->game_state.m_is_player_turn = true;
    this->game_state.m_player_ships->getNewDeads();
}

bool Game::isPlayerWin() {
    if (this->game_state.m_enemy_field->isAllShipsDead()){
        this->game_state.m_is_game_started = false;
        return true;
    }
    return false;
}

bool Game::isEnemyWin() {
    if (this->game_state.m_player_field->isAllShipsDead()){
        this->game_state.m_is_game_started = false;
        return true;
    }
    return false;
}

void Game::reload_enemy(shared_ptr<Battleground> battleground, shared_ptr<ShipsManager> ships_manager) {
    this->game_state.m_enemy_field = battleground;
    this->game_state.m_enemy_ships = ships_manager;
    this->start();
}

void Game::reload_game(shared_ptr<Battleground> player_field, shared_ptr<Battleground> enemy_field, shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships) {
    this->game_state.m_player_field = player_field;
    this->game_state.m_player_ships = player_ships;
    this->game_state.m_info_holder.battleground = enemy_field;
    this->game_state.m_info_holder.ships_manager = enemy_ships;
    this->game_state.m_info_holder.x = 0;
    this->game_state.m_info_holder.y = 0;
    this->game_state.m_ability_manager = AbilityManager();
    this->reload_enemy(enemy_field, enemy_ships);
}

bool Game::getIsPlayerTurn() {
    return this->game_state.m_is_player_turn;
}

bool Game::getIsGameStarted() {
    return this->game_state.m_is_game_started;
}

void Game::check_game_status(bool reverse) {
    if ((!this->game_state.m_is_player_turn && !reverse) || (this->game_state.m_is_player_turn && reverse))
        throw logic_error("Other players turn");
    if (!this->game_state.m_is_game_started)
        throw logic_error("Game is not started!");
}

void Game::save(string filename) {
    this->check_game_status();
    ofstream out(filename);
    out << this->game_state;
    out.close();
}

void Game::load(string filename) {
    this->check_game_status();
    ifstream in(filename);
    if (!in)
    	throw invalid_argument("Save not found!");
    in >> this->game_state;
    in.close();
}
