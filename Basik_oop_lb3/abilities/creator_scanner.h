#pragma once

#include "i_creator.h"
#include "i_ability.h"
#include "a_scanner.h"

using namespace std;


class CreatorScanner : public ICreator {
public:
    unique_ptr<IAbility> createAbility();
};