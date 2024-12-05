#include "a_random_attack.h"


bool RandomAttack::applyAbility(InfoHolder& info_holder){
    random_device rd;
    default_random_engine engine(rd());

    vector<pair<size_t, size_t>> idxs;

    for (size_t i = 0; i < info_holder.battleground->getHeight(); ++i)
        for (size_t j = 0; j < info_holder.battleground->getWidth(); ++j)
            if (!info_holder.battleground->getCell(j, i).isEmpty() &&
                info_holder.battleground->getCell(j, i).getSegment().getState() != Segment::State::DESTROYED)
                idxs.push_back(pair<size_t, size_t>(i, j));
    
    if (idxs.empty())
        return false;
    size_t idx = engine() % idxs.size();
    info_holder.battleground->attack(idxs[idx].second, idxs[idx].first);

    return true;
}