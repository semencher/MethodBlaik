#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>

#include <QVector>

#include <QString>

#include <fstream>

#include <QDebug>

#include "math.h"

class BoolVector;

class BoolInterval;
class BoolVector;

typedef unsigned char byte;

class BCException
{
    std::string type_error;

public:
    BCException()
    {
        type_error = "";
    }

    void setTypeError(std::string tmp)
    {
        type_error = tmp;
    }

    std::string message()
    {
        return type_error;
    }
};  // BCException

struct IntervalFunction
{
    BoolInterval * interval;
    bool value;
};

#endif // GLOBAL_H
