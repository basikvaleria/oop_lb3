#pragma once

#include <cstdlib>

#include "i_ability.h"

using namespace std;


class DoubleDamage : public IAbility {
    public:
        bool applyAbility(InfoHolder& info_holder);
};
