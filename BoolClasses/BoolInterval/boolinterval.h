#ifndef BOOLINTERVAL_H
#define BOOLINTERVAL_H

#include "global.h"

class BoolInterval;

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

    void correct(QVector<IntervalFunction *> & a0, int j);


    bool ortByOnlyComp(BoolInterval & second_op);
    bool ort(BoolInterval & second_op);
    IntervalFunction * generalBonding(BoolInterval & second_op);

};  // BoolInterval

#endif // BOOLINTERVAL_H
