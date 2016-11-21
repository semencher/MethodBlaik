#include <QCoreApplication>

#include "functionofmethod.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {


    QVector<IntervalFunction *> array;
    QVector<IntervalFunction *> a0;
    parse(argv[1], array, a0);

    int size = array.size();
    for (int i = 0; i < size; ++i)
    {
        array[i]->interval->showBI(std::cout);
        std::cout << " " << array[i]->value << "\n";

    }
     size = a0.size();
    for (int i = 0; i < size; ++i)
    {
        a0[i]->interval->showBI(std::cout);
        std::cout << " " << a0[i]->value << "\n";

    }
    std::cout << "\n\n\n\n";


        correction(array, a0);

         size = array.size();
        for (int i = 0; i < size; ++i)
        {
            array[i]->interval->showBI(std::cout);
            std::cout << " " << array[i]->value << "\n";

        }
         size = a0.size();
        for (int i = 0; i < size; ++i)
        {
            a0[i]->interval->showBI(std::cout);
            std::cout << " " << a0[i]->value << "\n";

        }
    std::cout << "\n\n\n\n";

    generalBonding(array);

     size = array.size();
    for (int i = 0; i < size; ++i)
    {
        array[i]->interval->showBI(std::cout);
        std::cout << " " << array[i]->value << "\n";

    }
     size = a0.size();
    for (int i = 0; i < size; ++i)
    {
        a0[i]->interval->showBI(std::cout);
        std::cout << " " << a0[i]->value << "\n";

    }

    std::cout << "\n\n\n\n";

    absorb(array);

     size = array.size();
    for (int i = 0; i < size; ++i)
    {
        array[i]->interval->showBI(std::cout);
        std::cout << " " << array[i]->value << "\n";

    }
size = a0.size();
    for (int i = 0; i < size; ++i)
    {
        a0[i]->interval->showBI(std::cout);
        std::cout << " " << a0[i]->value << "\n";

    }

    show(array, a0, argv[2]);


    }
    catch(BCException e)
    {
        std::cout << e.message();
    }

    return 0;
}
