#ifndef BOOLINTERVAL_H
#define BOOLINTERVAL_H

#include "global.h"

#include "BoolClasses/BoolVector/boolvector.h"

class BoolInterval
{
    BoolVector * bv;
    BoolVector * vv;

public:

    BoolInterval()
    {
        bv=new BoolVector();
        vv=new BoolVector();

        if (bv==NULL || vv==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }
    }

    BoolInterval(BoolInterval & second_op);
    BoolInterval(int n);
    void showBI(std::ostream & strm);

    ~BoolInterval()
    {
        if (bv)
        {
            delete bv;
        }
        if (vv)
        {
            delete vv;
        }
        bv=NULL;
        vv=NULL;
    }

    void set1(int k);
    void set0(int k);
    void setDC(int k);
    void setDC();
    int get(int k);
    int getSize();
    bool equal(BoolInterval &second_op);
    void assign(BoolInterval &second_op);
    int weight();
    bool absorb(BoolInterval &second_op);
    BoolInterval(BoolInterval &second_op, BoolInterval &third_op);
    int fullweight();

};  // BoolInterval

#endif // BOOLINTERVAL_H
