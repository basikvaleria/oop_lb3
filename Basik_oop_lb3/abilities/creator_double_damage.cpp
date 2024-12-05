#include "creator_double_damage.h"

unique_ptr<IAbility> CreatorDoubleDamage::createAbility(){
    return make_unique<DoubleDamage>();
}