#ifndef BOOLVECTOR_H
#define BOOLVECTOR_H

#include "global.h"

class BoolVector
{
    friend BoolInterval;

    byte * vect;
    int size;
    int size_arr;
    int tail;

public:
    BoolVector()
    {
        size =      0;
        vect =      NULL;
        size_arr =  0;
        tail =      0;
    }

    BoolVector(BoolVector & second_bv);
    BoolVector(int n);
    void showBV(std::ostream & strm);

    ~BoolVector()
    {
        if (vect)
            delete []vect;
        vect = NULL;
    }

    void set1(int k);
    void set0(int k);
    int get(int k);
    void inv(BoolVector & rezult);
    void andOp(BoolVector & second_op, BoolVector & rezult);
    void sumModTwo(BoolVector & second_op, BoolVector & rezult);

    int getSize()
    {
        return size;
    }

    void orOp(BoolVector & second_op, BoolVector & rezult);
    bool equal(BoolVector & second_op);
    void assign(BoolVector & second_op);
    void left(int k, BoolVector & rezult);
    void right(int k, BoolVector & rezult);
    int weight();

};  // BoolVector

#endif // BOOLVECTOR_H
