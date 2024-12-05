#pragma once

#include <stdexcept>

using namespace std;


class AttackOutOfBattlegroundExpection : public logic_error{
    public:
        AttackOutOfBattlegroundExpection():logic_error("Attack out of Battleground"){}
};