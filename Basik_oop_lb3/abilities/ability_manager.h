#pragma once

#include <algorithm>
#include <random>
#include <vector>

#include "i_creator.h"
#include "creator_random_attack.h"
#include "creator_double_damage.h"
#include "creator_scanner.h"
#include "no_ability_exception.h"

using namespace std;


class AbilityManager{
    public:
        AbilityManager();
        ICreator& getRandomCreator();
        unique_ptr<IAbility> getAbility();
        void addRandomAbility();
        void addRandomAbilites(size_t count);
        size_t len();
        size_t len() const;
        const vector<unique_ptr<IAbility>>& getSkills() const;
        void addAbility(size_t index);
    private:
        vector<unique_ptr<IAbility>> m_abilities;
        vector<unique_ptr<ICreator>> m_creators;
};