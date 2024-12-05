#include "segment.h"

Segment::Segment(): m_state{State::UNDAMAGED} {}

Segment::State Segment::getState(){
    return this->m_state;
}

void Segment::setState(State state){
    this->m_state = state;
}

void Segment::changeStateDelta(int delta){
    int new_value = m_state + delta;
    if (new_value >= Segment::State::UNDAMAGED)
        m_state = Segment::State::UNDAMAGED;
    else if (new_value <= Segment::State::DESTROYED)
        m_state = Segment::State::DESTROYED;
    else
        m_state = Segment::State::DAMAGED;
}
