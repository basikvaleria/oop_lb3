#pragma once

#include <vector>
#include <utility>
#include <random>
#include <iostream>

#include "i_ability.h"
#include "info_holder.h"
#include "ship.h"

using namespace std;


class RandomAttack: public IAbility{
    public:
        bool applyAbility(InfoHolder& info_holder);
};