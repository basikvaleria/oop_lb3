#pragma once

#include <memory>

#include "info_holder.h"

using namespace std;


class IAbility {
public:
    virtual ~IAbility() = default;
    virtual bool applyAbility(InfoHolder& info_holder) = 0;
};
