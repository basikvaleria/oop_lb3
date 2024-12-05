#include "a_double_damage.h"


bool DoubleDamage::applyAbility(InfoHolder& info_holder){
    info_holder.battleground->setAttackMultiplier(2);
    return true;
}