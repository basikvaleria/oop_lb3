#include "cell.h"


Cell::Cell(): m_is_empty{true}, m_state{Cell::ViewState::HIDDEN}, m_segment{nullptr} {}

bool Cell::isEmpty(){
    return this->m_is_empty;
}

void Cell::setSegment(Segment& segment){
    this->m_segment = &segment;
    this->m_is_empty = false;
}

Segment& Cell::getSegment(){
    if (isEmpty())
        throw invalid_argument("This cell is empty!");
    return *this->m_segment;
}

void Cell::setState(Cell::ViewState state){
    this->m_state = state;
}

Cell::ViewState Cell::getState(){
    return this->m_state;
}