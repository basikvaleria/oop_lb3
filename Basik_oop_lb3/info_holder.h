#pragma once

#include "battleground.h"
#include "ships_manager.h"
#include <memory> // Added to include shared_ptr


class InfoHolder{
    public:
        std::shared_ptr<Battleground> battleground;
        std::shared_ptr<ShipsManager> ships_manager;
        size_t x;
        size_t y;
        InfoHolder(std::shared_ptr<Battleground> battleground, std::shared_ptr<ShipsManager> ships_manager, size_t x=0, size_t y=0):
            battleground(battleground), ships_manager(ships_manager), x(x), y(y) {}
};