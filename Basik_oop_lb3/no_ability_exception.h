#pragma once

#include <stdexcept>

using namespace std;


class NoAbilityException : public runtime_error{
    public:
        NoAbilityException() : runtime_error("No abilities left"){}
};
