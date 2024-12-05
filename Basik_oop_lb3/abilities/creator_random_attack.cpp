#include "creator_random_attack.h"

unique_ptr<IAbility> CreatorRandomAttack::createAbility(){
    return make_unique<RandomAttack>();
}