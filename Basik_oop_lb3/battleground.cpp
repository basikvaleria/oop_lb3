#include "battleground.h"


Battleground::Battleground(size_t width, size_t height){
    this->m_width = width;
    this->m_height = height;
    this->m_attack_multiplier = 1;

    this->m_battleground = new Cell*[height];
    for (size_t i = 0; i < height; ++i)
        this->m_battleground[i] = new Cell[width]();
}

Battleground::Battleground(const Battleground& battleground){
    this->m_width = battleground.m_width;
    this->m_height = battleground.m_height;
    this->m_attack_multiplier = battleground.m_attack_multiplier;

    this->m_battleground = new Cell*[this->m_height];
    for (size_t i = 0; i < this->m_height; ++i){
        this->m_battleground[i] = new Cell[this->m_width];
        for (size_t j = 0; j < this->m_width; ++j)
            this->m_battleground[i][j] = battleground.m_battleground[i][j];
    }
}

Battleground& Battleground::operator=(const Battleground& battleground){
    if (this != &battleground){
        if (this->m_battleground){
            for (size_t i = 0; i < this->m_height; ++i)
                delete[] this->m_battleground[i];
            delete[] this->m_battleground;
        }

        this->m_width = battleground.m_width;
        this->m_height = battleground.m_height;
        this->m_attack_multiplier = battleground.m_attack_multiplier;

        this->m_battleground = new Cell*[this->m_height];
        for (size_t i = 0; i < this->m_height; ++i){
            this->m_battleground[i] = new Cell[this->m_width];
            for (size_t j = 0; j < this->m_width; ++j)
                this->m_battleground[i][j] = battleground.m_battleground[i][j];
        }
    }
    return *this;
}


Battleground::Battleground(Battleground&& battleground){
    this->m_width = battleground.m_width;
    this->m_height = battleground.m_height;
    this->m_battleground = battleground.m_battleground;
    this->m_attack_multiplier = battleground.m_attack_multiplier;

    battleground.m_width = 0;
    battleground.m_height = 0;
    battleground.m_battleground = nullptr;
    battleground.m_attack_multiplier = 0;
}

Battleground& Battleground::operator=(Battleground&& battleground){
    if (this != &battleground){
        if (this->m_battleground){
            for (size_t i = 0; i < this->m_height; ++i)
                delete[] this->m_battleground[i];
            delete[] this->m_battleground;
        }

        this->m_width = battleground.m_width;
        this->m_height = battleground.m_height;
        this->m_battleground = battleground.m_battleground;
        this->m_attack_multiplier = battleground.m_attack_multiplier;

        battleground.m_width = 0;
        battleground.m_height = 0;
        battleground.m_battleground = nullptr;
        battleground.m_attack_multiplier = 0;
    }
    return *this;
}

Cell& Battleground::getCell(size_t x, size_t y){
    if (x >= this->m_width || y >= this->m_height)
        throw CoordinatesException();

    return this->m_battleground[y][x];
}

size_t Battleground::getWidth(){
    return this->m_width;
}

size_t Battleground::getHeight(){
    return this->m_height;
}

Battleground::~Battleground(){
    if (this->m_battleground){
        for (size_t i = 0; i < this->m_height; ++i)
            delete [] this->m_battleground[i];
        delete [] this->m_battleground;
    }
}

bool Battleground::isCellFree(long int x, long int y){
    if (x >= (long int)this->m_width || x < 0 || y >= (long int)this->m_height || y < 0)
        return false;
    for (long int i = y - 1; i <= y + 1; ++i){
        for (long int j = x - 1; j <= x + 1; ++j){
            if (((0 <= j && j < (long int)this->m_width) && (0 <= i && i < (long int)this->m_height)) && !(this->m_battleground[i][j].isEmpty()))
                return false;
        }
    }

    return true;
}

void Battleground::addShip(size_t x, size_t y, Ship& ship, Ship::Orientation orientation){
    if (x >= this->m_width || y >= this->m_height)
        throw CoordinatesException();

    if (orientation == Ship::Orientation::VERTICAL){
        for (size_t i = y; i < y + ship.len(); ++i)
            if (!(this->isCellFree(x, i)))
                throw ShipPlacementException();

        for (size_t i = y; i < y + ship.len(); ++i)
            this->m_battleground[i][x].setSegment(ship.getSegment(i - y));
    }
    else{
        for (size_t i = x; i < x + ship.len(); ++i)
            if (!(this->isCellFree(i, y)))
                throw ShipPlacementException();
        
        for (size_t i = x; i < x + ship.len(); ++i)
            this->m_battleground[y][i].setSegment(ship.getSegment(i - x));
    }

    ship.setOrientation(orientation);
}

void Battleground::attack(size_t x, size_t y, size_t damage){
    if (x >= this->m_width || y >= this->m_height)
        throw AttackOutOfBattlegroundExpection();
    
    if (!(this->m_battleground[y][x].isEmpty())){
        this->m_battleground[y][x].getSegment().changeStateDelta(-damage*this->m_attack_multiplier);
        this->m_battleground[y][x].setState(Cell::ViewState::SHIP);
    }
    else{
        this->m_battleground[y][x].setState(Cell::ViewState::EMPTY);
    }
    this->m_attack_multiplier = 1;
}

void Battleground::setAttackMultiplier(size_t attack_multiplier){
    this->m_attack_multiplier = attack_multiplier;
}

bool Battleground::isAllShipsDead(){
    for (size_t i = 0; i < this->m_height; ++i)
        for (size_t j = 0; j < this->m_width; ++j)
            if (!this->m_battleground[i][j].isEmpty() && this->m_battleground[i][j].getSegment().getState() != Segment::State::DESTROYED)
                return false;
    return true;
}