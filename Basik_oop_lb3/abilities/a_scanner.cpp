#include "a_scanner.h"


bool Scanner::applyAbility(InfoHolder& info_holder){
    size_t y = min(info_holder.y + 2, info_holder.battleground->getHeight());
    size_t x = min(info_holder.x + 2, info_holder.battleground->getWidth());

    for (size_t i = info_holder.y; i < y; ++i)
        for (size_t j = info_holder.x; j < x; ++j)
            if (!info_holder.battleground->getCell(j, i).isEmpty())
                return true;

    return false;
}