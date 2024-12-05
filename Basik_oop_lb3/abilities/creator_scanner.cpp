#include "creator_scanner.h"

unique_ptr<IAbility> CreatorScanner::createAbility(){
    return make_unique<Scanner>();
}