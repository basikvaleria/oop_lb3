#include "ability_manager.h"


AbilityManager::AbilityManager() {

    m_creators.emplace_back(make_unique<CreatorDoubleDamage>());
    m_creators.emplace_back(make_unique<CreatorRandomAttack>());
    m_creators.emplace_back(make_unique<CreatorScanner>());
    
    for (auto& creator : m_creators){
        m_abilities.emplace_back(creator->createAbility());
    }

    random_device rd;
    default_random_engine engine(rd());
    shuffle(m_abilities.begin(), m_abilities.end(), engine);
}

void AbilityManager::addRandomAbility() {
    m_abilities.emplace_back(move(this->getRandomCreator().createAbility()));
}

void AbilityManager::addRandomAbilites(size_t count) {
    for (size_t i = 0; i < count; ++i)
        this->addRandomAbility();
}

ICreator& AbilityManager::getRandomCreator() {
    random_device rd;
    default_random_engine engine(rd());
    int index = engine() % m_creators.size();

    return *m_creators[index];
}

unique_ptr<IAbility> AbilityManager::getAbility() {
    if(m_abilities.size() == 0) {
        throw NoAbilityException();
    }
    auto ability = move(m_abilities[0]);
    m_abilities.erase(m_abilities.begin());
    return ability;
}

size_t AbilityManager::len() {
    return m_abilities.size();
}

size_t AbilityManager::len() const {
    return m_abilities.size();
}

const vector<unique_ptr<IAbility>>& AbilityManager::getSkills() const {
    return m_abilities;
}

void AbilityManager::addAbility(size_t index) {
    m_abilities.emplace_back(move(m_creators[index]->createAbility()));
}