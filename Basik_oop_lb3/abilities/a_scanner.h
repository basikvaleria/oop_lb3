#pragma once

#include <iostream>

#include "i_ability.h"
#include "info_holder.h"
#include "battleground.h"
#include "coordinates_exception.h"

using namespace std;


class Scanner: public IAbility{
public:
    bool applyAbility(InfoHolder& info_holder);
};