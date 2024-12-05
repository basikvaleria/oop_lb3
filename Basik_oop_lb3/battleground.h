#pragma once

#include "segment.h"
#include "ship.h"
#include "cell.h"
#include "attack_out_of_battleground_exception.h"
#include "coordinates_exception.h"
#include "ship_placement_exception.h"

#include <cstring>

using namespace std;


class Battleground{
    public:
        Battleground(size_t width, size_t height);
        Battleground(const Battleground& battleground);
        Battleground& operator=(const Battleground& battleground);
        Battleground(Battleground&& battleground);
        Battleground& operator=(Battleground&& battleground);
        ~Battleground();

        void addShip(size_t x, size_t y, Ship& ship, Ship::Orientation orientation);
        void attack(size_t x, size_t y, size_t damage=1);
        Cell& getCell(size_t x, size_t y);
        size_t getWidth();
        size_t getHeight();
        void setAttackMultiplier(size_t multiplier);
        bool isAllShipsDead();

    private:
        size_t m_width;
        size_t m_height;
        Cell** m_battleground;
        size_t m_attack_multiplier;

        bool isCellFree(long int x, long int y);
};