#pragma once


#include "battleground.h"
#include "ships_manager.h"
#include "abilities/ability_manager.h"
#include "ships_manager.h"
#include "info_holder.h"
#include <cxxabi.h>
#include <memory>

class Game;


class GameState {
    friend class Game;

    public:
        GameState(shared_ptr<Battleground> player_field, shared_ptr<Battleground> enemy_field, shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships);
        friend ostream& operator<<(ostream& os, const GameState& game_state);
        friend istream& operator>>(istream& is, GameState& game_state);

    private:
        shared_ptr<Battleground> m_player_field;
        shared_ptr<Battleground> m_enemy_field;
        shared_ptr<ShipsManager> m_player_ships;
        shared_ptr<ShipsManager> m_enemy_ships;
        InfoHolder m_info_holder;
        AbilityManager m_ability_manager;
        bool m_is_player_turn = true;
        bool m_is_game_started = false;

        vector<pair<size_t, size_t>> getShipPosition(Ship& ship, Battleground& field) const;
        string serializeShips(ShipsManager& ships_manager, Battleground& field) const;
        string serializeField(Battleground& field) const;
        string serializeSkills(const AbilityManager& ability_manager) const;
        vector<string> split(const string &s, char delim);
        vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>> readShips(vector<string>& lines, size_t& j);
        void updateFieldWithShips(Battleground& field, ShipsManager& ships_manager, vector<tuple<size_t, Ship::Orientation, vector<tuple<size_t, size_t, Segment::State>>>>& ships, vector<string>& lines, size_t& j);
};