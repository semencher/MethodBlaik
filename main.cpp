#include <QCoreApplication>

#include "functionofmethod.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {


    QVector<IntervalFunction> array = parse("file.txt");

    int size = array.size();
    for (int i = 0; i < size; ++i)
    {
        array[i].interval->showBI(std::cout);
        std::cout << " " << array[i].value << "\n";

    }
    std::cout << "\n\n\n\n";
    generalBonding(array);
    for (int i = 0; i < size; ++i)
    {
        array[i].interval->showBI(std::cout);
        std::cout << " " << array[i].value << "\n";

    }


    }
    catch(BCException e)
    {
        std::cout << e.message();
    }

    return a.exec();
}
