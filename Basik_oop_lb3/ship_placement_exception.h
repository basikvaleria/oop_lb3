#pragma once

#include <stdexcept>

using namespace std;


class ShipPlacementException: public runtime_error{
    public:
        ShipPlacementException() : runtime_error("Cant place ship! Ship placement is too close to each other"){}
};