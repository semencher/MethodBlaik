#include "functionofmethod.h"

void parse(const QString & plaFile, QVector<IntervalFunction *> & arrayIntFunc,
           QVector<IntervalFunction *> & a0)
{
    std::ifstream file;
    std::string nameFile = plaFile.toStdString();
    file.open(nameFile, std::ios::in);

    if (!file)
    {
        BCException e;
        e.setTypeError("Unable to open file!");
        throw e;
    }

    int icount=0;
    int ocount=0;
    int pcount=0;
    bool flagTypeFR=false;
    std::string a="";

    BCException e;
    e.setTypeError("Error of data!");

    char point;
    if (file.eof())
    {
        throw e;
    }
    file >> point;

    while (point=='.')
    {
        if (file.eof())
        {
            throw e;
        }
        file >> point;
        if (file.eof())
        {
            throw e;
        }
        switch (point)
        {
            case 'i':
                file >> icount;
                if (icount<=0)
                    throw e;
                break;
            case 'o':
                file >> ocount;
                if (ocount<=0)
                    throw e;
                break;
            case 'p':
                file >> pcount;
                if (pcount<=0)
                    throw e;
                break;
            case 't':
                a=a+point;
                while (point!=10)
                {
                    if (file.eof())
                    {
                        throw e;
                    }
                    point=file.get();
                    a=a+point;
                }
                if (a[0]=='t' && a[1]=='y' && a[2]=='p' && a[3]=='e' && a[4]==' ' && a[5]=='f' && a[6]=='r')
                    flagTypeFR=true;
                break;
            default:
                {
                    while (point!=10)
                    {
                        if (file.eof())
                        {
                            throw e;
                        }
                        point=file.get();
                    }
                }
        }

        if (file.eof())
        {
            throw e;
        }
        file >> point;
    }
    if (!flagTypeFR)
    {
        throw e;
    }

    BoolVector haract(ocount);
    BoolInterval * interv = new BoolInterval(icount);

    if (point=='1')
    {
        interv->set1(1);
    }
    else
    {
        if (point=='-')
            interv->setDC(1);
    }

    int i=1;

    for (int k=0; k<pcount; k++)
    {
        for (; i<icount; i++)
        {
            if (file.eof())
            {
                throw e;
            }
            file >> point;

            if (point=='1')
            {
                interv->set1(i+1);
            }
            else
            {
                if (point == '0')
                    interv->set0(i+1);
                else
                    interv->setDC(i+1);
            }
        }

        for (int j=0; j<ocount; j++)
        {
            if (file.eof())
            {
                throw e;
            }
            file >> point;

            if (point=='1')
            {
                haract.set1(j+1);
            }
            else
                haract.set0(j+1);
        }
        i=0;

        IntervalFunction * iF = new IntervalFunction;
        iF->interval = interv;
        iF->value = haract.get(1);
        interv = new BoolInterval(icount);
        if (iF->value == 1)
        {
            arrayIntFunc.push_back(iF);
        }
        else
        {
            a0.push_back(iF);
        }
    }
    file.close();
}

void generalBonding(QVector<IntervalFunction *> & arrayIntFunc)
{

    for(int i = 0; i < arrayIntFunc.size(); i++)
    {
        int j = i;
        j++;
        IntervalFunction * intervalI = arrayIntFunc[i];
        for(; j < arrayIntFunc.size(); j++)
        {
            IntervalFunction * intervalJ = arrayIntFunc[j];

            if (intervalI->interval->ortByOnlyComp(*(intervalJ->interval)))
            {
                IntervalFunction * tmp = intervalI->interval->generalBonding(*(intervalJ->interval));
                bool a = true;
                for (int k=0; k<arrayIntFunc.size(); k++)
                {
                    if (arrayIntFunc[k]->interval->equal(*(tmp->interval)))
                        a = false;
                }
                if (a)
                    arrayIntFunc.push_back(tmp);
            }
        }
    }
}

void absorb(QVector<IntervalFunction *> & arrayIntFunc)
{
    for(int i = 0; i < arrayIntFunc.size(); i++)
    {
        int j = i;
        j++;
        IntervalFunction * intervalI = arrayIntFunc[i];
        for(; j < arrayIntFunc.size(); j++)
        {
            IntervalFunction * intervalJ = arrayIntFunc[j];

            if (intervalI->interval->absorb(*(intervalJ->interval)))
            {
                arrayIntFunc.remove(j);
                j--;
            }
            else
            {
                if (intervalJ->interval->absorb(*(intervalI->interval)))
                {
                    arrayIntFunc.remove(i);
                    i--;
                    break;
                }
            }
        }
    }
}

void correction(QVector<IntervalFunction *> & a1, QVector<IntervalFunction *> & a0)
{
    for (int i = 0; i < a1.size(); i++)
    {
        IntervalFunction * intA1 = a1[i];
        for (int j = 0; j < a0.size(); j++)
        {
            IntervalFunction * intA0 = a0[j];
            if(!intA1->interval->ort(*(intA0->interval)))
            {
                intA1->interval->correct(a0, j);
                j--;
            }
        }
    }
}

void show(QVector<IntervalFunction *> & a1, QVector<IntervalFunction *> & a0, std::string filename)
{
    std::ofstream file;
    file.open(filename, std::ios::out);

    if (!file)
    {
        BCException e;
        e.setTypeError("Unable to open file!");
        throw e;
    }

    file << "type fr\n";
    file << ".i " << a1[0]->interval->getSize() << "\n";
    file << ".o 1\n";
    file << ".p " << a1.size()+a0.size() << "\n";

    for(int i=0; i<a1.size(); i++)
    {
        a1[i]->interval->showBI(file);
        file << " 1\n";
    }
    for(int j=0; j<a0.size(); j++)
    {
        a0[j]->interval->showBI(file);
        file << " 0\n";
    }

    file << ".e";
    file.close();
}
