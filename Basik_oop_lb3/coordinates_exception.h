#pragma once

#include <stdexcept>

using namespace std;


class CoordinatesException: public logic_error{
    public:
        CoordinatesException(): logic_error("Coordinates are out of Battleground"){}
};
