#include "segment.h"
#include "ships_manager.h"
#include "ship.h"
#include "battleground.h"
#include "info_holder.h"
#include "abilities/ability_manager.h"
#include "game.h"

#include <iostream>

using namespace std;

int main(){
    try{
        // Initialize ship lengths and create ship managers for both players
        vector<size_t> lens = {1, 2, 3, 4, 4};
        ShipsManager player_manager(5, lens);
        ShipsManager enemy_manager(5, lens);

        // Create battlegrounds for both players
        Battleground playground_player(10, 10);
        Battleground playground_enemy(10, 10);

        // Use shared pointers for managing resources
        auto playground_player_ptr = make_shared<Battleground>(playground_player);
        auto player_manager_ptr = make_shared<ShipsManager>(player_manager);
        auto playground_enemy_ptr = make_shared<Battleground>(playground_enemy);
        auto enemy_manager_ptr = make_shared<ShipsManager>(enemy_manager);

        playground_player_ptr->addShip(0, 0, player_manager_ptr->getShip(4), Ship::Orientation::VERTICAL);
        playground_enemy_ptr->addShip(1, 0, enemy_manager_ptr->getShip(0), Ship::Orientation::HORIZONTAL);

        Game game(playground_player_ptr, playground_enemy_ptr, player_manager_ptr, enemy_manager_ptr);
        game.start();

        // Load game state from a file
        game.load("save.txt");

        // Simulate player and enemy turns
        game.playerTurn(1, 0);
        game.enemyTurn();
        game.playerTurn(1, 0);
        game.enemyTurn();

        // Save the current game state
        game.save("saveee.txt");

        // Display game results
        cout << "Player Win: " << game.isPlayerWin() << endl;
        cout << "Enemy Win: " << game.isEnemyWin() << endl;
    }
    catch (exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
