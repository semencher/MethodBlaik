#include "BoolInterval.h"

BoolInterval::BoolInterval(BoolInterval & second_op)
{
    bv=new BoolVector(*second_op.bv);
    vv=new BoolVector(*second_op.vv);

    if (bv==NULL || vv==NULL)
    {
        BCException e;
        e.setTypeError("Memory is not allocated!");
        throw e;
    }
}

BoolInterval::BoolInterval(int n)
{
    bv=new BoolVector(n);
    vv=new BoolVector(n);

    if (bv==NULL || vv==NULL)
    {
        BCException e;
        e.setTypeError("Memory is not allocated!");
        throw e;
    }
}

void BoolInterval::showBI(std::ostream & strm)
{
    int k=bv->getSize();

    for (int i=1; i<=k; i++)
    {
        if (bv->get(i)==1)
            strm << "1";
        else
            if (vv->get(i)==1)
                strm << "-";
            else
                strm << "0";
    }
}

void BoolInterval::set1(int k)
{
    if (k>bv->size || k<1)
    {
        BCException e;
        e.setTypeError("Index value error!");
        throw e;
    }
    else
    {
        int full_byte=(int)ceil((double)k/8);
        int num_ind=(k-1) % 8;
        byte mask=1;
        mask=mask << num_ind;
        bv->vect[full_byte-1]=bv->vect[full_byte-1] | mask;
        mask=~mask;
        vv->vect[full_byte-1]=vv->vect[full_byte-1] & mask;
    }
}

void BoolInterval::set0(int k)
{
    if (k>bv->size || k<1)
    {
        BCException e;
        e.setTypeError("Index value error!");
        throw e;
    }
    else
    {
        int full_byte=(int)ceil((double)k/8);
        int num_ind=(k-1) % 8;
        byte mask=1;
        mask=mask << num_ind;
        mask=~mask;
        vv->vect[full_byte-1]=vv->vect[full_byte-1] & mask;
        bv->vect[full_byte-1]=bv->vect[full_byte-1] & mask;
    }
}

void BoolInterval::setDC(int k)
{
    if (k>bv->size || k<1)
    {
        BCException e;
        e.setTypeError("Index value error!");
        throw e;
    }
    else
    {
        int full_byte=(int)ceil((double)k/8);
        int num_ind=(k-1) % 8;
        byte mask=1;
        mask=mask << num_ind;
        vv->vect[full_byte-1]=vv->vect[full_byte-1] | mask;
        mask=~mask;
        bv->vect[full_byte-1]=bv->vect[full_byte-1] & mask;
    }
}

int BoolInterval::get(int k)
{
    if (k>bv->size || k<1)
    {
        BCException e;
        e.setTypeError("Index value error!");
        throw e;
    }
    else
    {
        int full_byte=(int)ceil((double)k/8);
        int num_ind=(k-1) % 8;
        byte mask=1;
        mask=mask << num_ind;
        if (bv->vect[full_byte-1] & mask)
            return 1;
        else
            if (vv->vect[full_byte-1] & mask)
                return -1;
            else
                return 0;
    }
}

int BoolInterval::getSize()
{
    return bv->getSize();
}

bool BoolInterval::equal(BoolInterval &second_op)
{
    if (second_op.bv->size!=bv->size)
    {
        return false;
    }
    for (int i=0; i<bv->size_arr; i++)
    {
        if (second_op.bv->vect[i] != bv->vect[i] || second_op.vv->vect[i] != vv->vect[i])
            return false;
    }
    return true;
}

void BoolInterval::assign(BoolInterval &second_op)
{
    bv->assign(*second_op.bv);
    vv->assign(*second_op.vv);
}

int BoolInterval::weight()
{
    return bv->weight();
}

bool BoolInterval::absorb(BoolInterval &second_op)
{
    BoolVector tmp;
    vv->andOp(*(second_op.vv), tmp);
    if (!tmp.equal(*(second_op.vv)))
        return false;
    vv->inv(tmp);
    second_op.bv->andOp(tmp, tmp);
    if (tmp.equal(*bv))
        return true;
    else
        return false;
}

BoolInterval::BoolInterval(BoolInterval &second_op, BoolInterval &third_op)
{
    if (second_op.bv->size != third_op.bv->size)
    {
        BCException e;
        e.setTypeError("Sizes are not correct!");
        throw e;
    }

    bv=new BoolVector(second_op.bv->size);
    vv=new BoolVector(second_op.bv->size);

    if (bv==NULL || vv==NULL)
    {
        BCException e;
        e.setTypeError("Memory is not allocated!");
        throw e;
    }

    for (int i=0; i<bv->size_arr; i++)
    {
        bv->vect[i]=second_op.bv->vect[i] ^ third_op.bv->vect[i];
        bv->vect[i]=bv->vect[i] & (~second_op.vv->vect[i]);
        bv->vect[i]=bv->vect[i] & (~third_op.vv->vect[i]);
        vv->vect[i]=~bv->vect[i];
    }
    if (vv->tail>0)
    {
        vv->vect[vv->size_arr-1]=vv->vect[vv->size_arr-1] << (8-vv->tail);
        vv->vect[vv->size_arr-1]=vv->vect[vv->size_arr-1] >> (8-vv->tail);
    }
    if (weight()==0)
    {
        BCException e;
        e.setTypeError("Sets M1 and M0 intersect!");
        throw e;
    }
}

int BoolInterval::fullweight()
{
    BoolVector a;
    vv->inv(a);
    return a.weight();
}

void BoolInterval::setDC()
{
    bv->sumModTwo(*bv,*bv);
    vv->sumModTwo(*vv,*vv);
    vv->inv(*vv);
}

bool BoolInterval::ortByOnlyComp(BoolInterval & second_op)
{
    BoolVector sumVV(second_op.vv->getSize());
    vv->orOp(*second_op.vv, sumVV);
    sumVV.inv(sumVV);
    BoolVector sumModTwo(second_op.vv->getSize());
    bv->sumModTwo(*second_op.bv, sumModTwo);
    BoolVector rezult(vv->getSize());
    sumModTwo.andOp(sumVV, rezult);
    if (rezult.weight() == 1)
    {
        return true;
    }
    else
        return false;
}

IntervalFunction * BoolInterval::generalBonding(BoolInterval & second_op)
{
    BoolVector conVV(vv->getSize());
    vv->andOp(*second_op.vv, conVV);
    BoolVector invConVV(vv->getSize());
    conVV.inv(invConVV);
    BoolVector sumBV(vv->getSize());
    bv->orOp(*second_op.bv, sumBV);
    sumBV.andOp(invConVV, sumBV);

    BoolVector sumVV(vv->getSize());
    vv->orOp(*second_op.vv, sumVV);
    BoolVector invSumVV(vv->getSize());
    sumVV.inv(invSumVV);
    BoolVector sumModTwo1(vv->getSize());
    bv->sumModTwo(*second_op.bv, sumModTwo1);
    BoolVector addInt(vv->getSize());
    sumModTwo1.andOp(invSumVV, addInt);
    BoolVector invAddInt(vv->getSize());
    addInt.inv(invAddInt);

    IntervalFunction * intFunc = new IntervalFunction;
    intFunc->value = true;
    sumBV.andOp(invAddInt ,*intFunc->interval->bv);
    conVV.orOp(addInt ,*intFunc->interval->vv);

    return intFunc;
}
