#pragma once

#include <memory>

#include "i_ability.h"

using namespace std;


class ICreator{
public:
    virtual ~ICreator() = default;
    virtual unique_ptr<IAbility> createAbility() = 0;
};
