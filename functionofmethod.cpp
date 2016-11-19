#include "functionofmethod.h"

QVector parse(const QString & plaFile)
{
    QVector<IntervalFunction> arrayIntFunc;
    std::ifstream file;
    file.open(namefile, std::ios::in);

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
    BoolInterval interv(icount);

    if (point=='1')
    {
        interv.set1(1);
    }

    int i=1;

    for (int k=0; k<pcount; k++)
    {
        for (i; i<icount; i++)
        {
            if (file.eof())
            {
                throw e;
            }
            file >> point;

            if (point=='1')
            {
                interv.set1(i+1);
            }
            else
                interv.set0(i+1);
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

        IntervalFunction iF;
        iF.interval = new BoolInterval(interv);
        iF.value = haract.get(1);

        arrayIntFunc.push_back(iF);
    }
    return arrayIntFunc;
}
