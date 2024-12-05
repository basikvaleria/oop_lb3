#pragma once


#include "battleground.h"
#include "ships_manager.h"
#include "abilities/ability_manager.h"
#include "ships_manager.h"
#include "info_holder.h"
#include "game_state.h"
#include <memory> // Added include for shared_ptr


class Game {
    public:
        Game(shared_ptr<Battleground> player_field, shared_ptr<Battleground> enemy_field, shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships);
        void start();
        void playerTurn(size_t x, size_t y, bool use_skill=false, size_t skill_x=0, size_t skill_y=0);
        void enemyTurn();
        bool isPlayerWin();
        bool isEnemyWin();
        void reload_enemy(shared_ptr<Battleground> battleground, shared_ptr<ShipsManager> ships_manager);
        void reload_game(shared_ptr<Battleground> player_field, shared_ptr<Battleground> enemy_field, shared_ptr<ShipsManager> player_ships, shared_ptr<ShipsManager> enemy_ships);
        bool getIsPlayerTurn();
        bool getIsGameStarted();
        void save(string filename);
        void load(string filename);

    private:
        GameState game_state;

        void check_game_status(bool reverse=false);
};