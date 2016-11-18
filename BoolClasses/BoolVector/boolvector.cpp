#include "BoolVector.h"

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
