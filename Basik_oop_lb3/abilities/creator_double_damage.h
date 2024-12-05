#pragma once

#include <memory>

#include "i_creator.h"
#include "a_double_damage.h"

using namespace std;


class CreatorDoubleDamage : public ICreator {
public:
    unique_ptr<IAbility> createAbility();
};
