#pragma once

#include <memory>

#include "a_random_attack.h"
#include "i_ability.h"
#include "i_creator.h"

using namespace std;


class CreatorRandomAttack: public ICreator {
public:
    unique_ptr<IAbility> createAbility();
};