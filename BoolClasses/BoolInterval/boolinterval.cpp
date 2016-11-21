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
    BoolVector invSumVV(vv->getSize());
    sumVV.inv(invSumVV);
    BoolVector sumModTwo(second_op.vv->getSize());
    bv->sumModTwo(*second_op.bv, sumModTwo);
    BoolVector rezult(vv->getSize());
    sumModTwo.andOp(invSumVV, rezult);
    if (rezult.weight() == 1)
    {
        return true;
    }
    else
        return false;
}

bool BoolInterval::ort(BoolInterval & second_op)
{
    BoolVector sumVV(second_op.vv->getSize());
    vv->orOp(*second_op.vv, sumVV);
    BoolVector invSumVV(vv->getSize());
    sumVV.inv(invSumVV);
    BoolVector sumModTwo(second_op.vv->getSize());
    bv->sumModTwo(*second_op.bv, sumModTwo);
    BoolVector rezult(vv->getSize());
    sumModTwo.andOp(invSumVV, rezult);
    if (rezult.weight() >= 1)
    {
        return true;
    }
    else
        return false;
}

void BoolInterval::correct(QVector<IntervalFunction *> & a0, int j)
{
    BoolVector sumModVV(vv->getSize());
    IntervalFunction * intA0 = a0[j];

    vv->sumModTwo(*(intA0->interval->vv), sumModVV);
    BoolVector sumModVVandVV0(vv->getSize());
    intA0->interval->vv->andOp(sumModVV, sumModVVandVV0);

    if (sumModVVandVV0.weight() > 0)
    {
        for(int i = 0; i < sumModVVandVV0.getSize(); i++)
        {
            if (sumModVVandVV0.get(i+1) == 1)
            {
                IntervalFunction * tmpIntFunc = new IntervalFunction;
                tmpIntFunc->interval = new BoolInterval(*(intA0->interval));
                tmpIntFunc->value = false;
                if(bv->get(i+1) == 1)
                {
                   tmpIntFunc->interval->set0(i+1);
                }
                else
                {
                    tmpIntFunc->interval->set1(i+1);
                }
                a0.push_back(tmpIntFunc);
            }
        }
    }
    delete intA0->interval;
    a0.remove(j);
}

IntervalFunction * BoolInterval::generalBonding(BoolInterval & second_op)
{
    BoolVector conVV(vv->getSize());
    vv->andOp(*second_op.vv, conVV);
    BoolVector invConVV(vv->getSize());
    conVV.inv(invConVV);
    BoolVector sumBV(vv->getSize());
    bv->orOp(*second_op.bv, sumBV);
    BoolVector sumBVAndInvConVV(vv->getSize());
    sumBV.andOp(invConVV, sumBVAndInvConVV);    //sumBV

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
    intFunc->interval = new BoolInterval(vv->getSize());
    intFunc->value = true;
    sumBVAndInvConVV.andOp(invAddInt ,*(intFunc->interval->bv));
    conVV.orOp(addInt ,*(intFunc->interval->vv));

    return intFunc;
}



BoolVector::BoolVector(int n)
{
    if (n < 0)
        size = abs(n);
    else
        size = n;
    size_arr = (int)ceil((double)size/8);
    tail = size % 8;
    vect = NULL;
    if (size > 0)
    {
        vect = new byte[size_arr];
        if (vect != NULL)
        {
            for (int i = 0; i < size_arr; ++i)
                vect[i] = vect[i] ^ vect[i];
        }
        else
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }
    }
}

void BoolVector::showBV(std::ostream & strm)
{
    byte mask;
    byte rez;
    int tmp_size = (int)floor((double)size/8);

    for (int i = 0; i < tmp_size; ++i)
    {
        mask = 1;
        for (int j = 0; j < 8; ++j)
        {
            rez = mask & vect[i];
            if (rez > 0)
                strm << "1";
            else
                strm << "0";
            mask = mask << 1;
        }
    }

    mask = 1;
    int ind_of_last = size_arr-1;

    for (int i = 0; i < tail; ++i)
    {
        rez=mask & vect[ind_of_last];
        if (rez>0)
            strm << "1";
        else
            strm << "0";
        mask=mask << 1;
    }
}

void BoolVector::set1(int k)
{
    if (k>size || k<1)
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
        vect[full_byte-1]=vect[full_byte-1] | mask;
    }
}

void BoolVector::set0(int k)
{
    if (k>size || k<1)
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
        vect[full_byte-1]=vect[full_byte-1] & mask;
    }
}

int BoolVector::get(int k)
{
    if (k>size || k<1)
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
        if (vect[full_byte-1] & mask)
            return 1;
        else
            return 0;
    }
}

void BoolVector::inv(BoolVector &rezult)
{
    if (rezult.size!=size)
    {
        if (rezult.size>0)
        {
            delete [] rezult.vect;
            rezult.vect=NULL;
        }

        rezult.vect=new byte[size_arr];

        if (rezult.vect==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }

        rezult.size=size;
        rezult.size_arr=size_arr;
        rezult.tail=tail;
    }

    for (int i=0; i<size_arr; i++)
    {
        rezult.vect[i]=~vect[i];
    }

    if (tail>0)
    {
        rezult.vect[size_arr-1]=rezult.vect[size_arr-1] << (8-tail);
        rezult.vect[size_arr-1]=rezult.vect[size_arr-1] >> (8-tail);
    }
}

void BoolVector::andOp(BoolVector &second_op, BoolVector &rezult)
{
    if (second_op.size!=size)
    {
        BCException e;
        e.setTypeError("Sizes are not correct!");
        throw e;
    }

    if (rezult.size!=size)
    {
        if (rezult.size>0)
        {
            delete [] rezult.vect;
            rezult.vect=NULL;
        }

        rezult.vect=new byte[size_arr];

        if (rezult.vect==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }

        rezult.size=size;
        rezult.size_arr=size_arr;
        rezult.tail=tail;
    }

    for (int i=0; i<size_arr; i++)
    {
        rezult.vect[i]=second_op.vect[i] & vect[i];
    }
}

void BoolVector::sumModTwo(BoolVector &second_op, BoolVector &rezult)
{
    if (second_op.size!=size)
    {
        BCException e;
        e.setTypeError("Sizes are not correct!");
        throw e;
    }

    if (rezult.size!=size)
    {
        if (rezult.size>0)
        {
            delete [] rezult.vect;
            rezult.vect=NULL;
        }

        rezult.vect=new byte[size_arr];

        if (rezult.vect==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }

        rezult.size=size;
        rezult.size_arr=size_arr;
        rezult.tail=tail;
    }

    for (int i=0; i<size_arr; i++)
    {
        rezult.vect[i]=second_op.vect[i] ^ vect[i];
    }
}

void BoolVector::orOp(BoolVector &second_op, BoolVector &rezult)
{
    if (second_op.size!=size)
    {
        BCException e;
        e.setTypeError("Sizes are not correct!");
        throw e;
    }

    if (rezult.size!=size)
    {
        if (rezult.size>0)
        {
            delete [] rezult.vect;
            rezult.vect=NULL;
        }

        rezult.vect=new byte[size_arr];

        if (rezult.vect==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }

        rezult.size=size;
        rezult.size_arr=size_arr;
        rezult.tail=tail;
    }

    for (int i=0; i<size_arr; i++)
    {
        rezult.vect[i]=second_op.vect[i] | vect[i];
    }
}

bool BoolVector::equal(BoolVector &second_op)
{
    if (second_op.size!=size)
    {
        return false;
    }
    for (int i=0; i<size_arr; i++)
    {
        if (second_op.vect[i] != vect[i])
            return false;
    }
    return true;
}

void BoolVector::assign(BoolVector &second_op)
{
    this->~BoolVector();
    size=second_op.size;
    size_arr=second_op.size_arr;
    tail=second_op.tail;
    vect=NULL;
    vect=new byte[size_arr];

    if (vect==NULL)
    {
        BCException e;
        e.setTypeError("Memory is not allocated!");
        throw e;
    }

    for (int i=0; i<size_arr; i++)
        vect[i]=second_op.vect[i];
}

BoolVector::BoolVector(BoolVector &second_bv)
{
    size=second_bv.size;
    size_arr=second_bv.size_arr;
    tail=second_bv.tail;

    vect=new byte[size_arr];

    if (vect==NULL)
    {
        BCException e;
        e.setTypeError("Memory is not allocated!");
        throw e;
    }

    for (int i=0; i<size_arr; i++)
        vect[i]=second_bv.vect[i];
}

void BoolVector::left(int k, BoolVector &rezult)
{
    k=abs(k);

    if (size!=rezult.size)
    {
        if (rezult.size>0)
        {
            delete [] rezult.vect;
            rezult.vect=NULL;
        }

        rezult.vect=new byte[size_arr];
        if (rezult.vect==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }
        rezult.size=size;
        rezult.size_arr=size_arr;
        rezult.tail=tail;
    }

    for (int i=0; i<size_arr; i++)
            rezult.vect[i]=vect[i];

    if (k>size)
    {
        for (int i=0; i<size_arr; i++)
            rezult.vect[i]=rezult.vect[i] ^ rezult.vect[i];
        return;
    }

    int shift=(int)floor((double)k/8);
    if (k>8)
    {
        for (int i=0; i<size_arr-shift; i++)
        {
            rezult.vect[i]=rezult.vect[i+shift];
        }
        for (int i=size_arr-shift; i<size_arr; i++)
        {
            rezult.vect[i]=rezult.vect[i] ^ rezult.vect[i];
        }
    }

    int corr=k%8;

    if (corr>0)
    {
        byte tmp=0;

        for (int i=0; i<size_arr-shift-1; i++)
        {
            rezult.vect[i]=rezult.vect[i] >> corr;
            tmp=0;
            tmp=~tmp;
            tmp=tmp & rezult.vect[i+1];
            tmp=tmp << (8-corr);
            rezult.vect[i]=rezult.vect[i] | tmp;
        }
        rezult.vect[size_arr-shift-1]=rezult.vect[size_arr-shift-1] >> corr;
    }
}

void BoolVector::right(int k, BoolVector &rezult)
{
    k=abs(k);

    if (size!=rezult.size)
    {
        if (rezult.size>0)
        {
            delete [] rezult.vect;
            rezult.vect=NULL;
        }

        rezult.vect=new byte[size_arr];
        if (rezult.vect==NULL)
        {
            BCException e;
            e.setTypeError("Memory is not allocated!");
            throw e;
        }
        rezult.size=size;
        rezult.size_arr=size_arr;
        rezult.tail=tail;
    }

    for (int i=0; i<size_arr; i++)
        rezult.vect[i]=vect[i];

    if (k>size)
    {
        for (int i=0; i<size_arr; i++)
            rezult.vect[i]=rezult.vect[i] ^ rezult.vect[i];
        return;
    }

    int shift=(int)floor((double)k/8);
    if (k>8)
    {
        for (int i=size_arr-1; i>shift-1; i--)
        {
            rezult.vect[i]=vect[i-shift];
        }
        for (int i=0; i<shift; i++)
        {
            rezult.vect[i]=rezult.vect[i] ^ rezult.vect[i];
        }
    }

    int corr=k%8;

    if (corr>0)
    {
        byte tmp=0;

        for (int i=size_arr-1; i>shift; i--)
        {
            rezult.vect[i]=rezult.vect[i] << corr;
            tmp=0;
            tmp=~tmp;
            tmp=tmp & rezult.vect[i-1];
            tmp=tmp >> (8-corr);
            rezult.vect[i]=rezult.vect[i] | tmp;
        }
        rezult.vect[shift]=rezult.vect[shift] << corr;
    }

    if (tail>0)
    {
        rezult.vect[size_arr-1]=rezult.vect[size_arr-1] << (8-tail);
        rezult.vect[size_arr-1]=rezult.vect[size_arr-1] >> (8-tail);
    }
}

int BoolVector::weight()
{
    unsigned long int t=0;
    int wht=0;

    for (int i=0; i<size_arr; i++)
    {
        byte tmp=vect[i];
        t=(unsigned long int)tmp;

        t=(t & 0x55555555) + ((t >> 1) & 0x55555555);
        t=(t & 0x33333333) + ((t >> 2) & 0x33333333);
        t=(t & 0x0F0F0F0F) + ((t >> 4) & 0x0F0F0F0F);
        t=(t & 0x00FF00FF) + ((t >> 8) & 0x00FF00FF);
        t=(t & 0x0000FFFF) + ((t >> 16) & 0x0000FFFF);

        wht=wht+t;
    }
    return wht;
}
