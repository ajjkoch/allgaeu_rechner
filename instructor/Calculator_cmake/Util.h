#ifndef UTIL_H
#define UTIL_H

#include "QObject"

enum E_OP_ID
{
    ADD,
    MULT,
    DIV,
    SUB
};

double calculate(double first, double second, E_OP_ID eOpId)
{
    switch(eOpId)
    {
    case ADD:
        return first + second;
    case MULT:
        return first * second;
    case DIV:
        return first / second;
    case SUB:
        return first - second;
    }
}

#endif // UTIL_H
