#ifndef FUNCTIONOFMETHOD_H
#define FUNCTIONOFMETHOD_H

#include "global.h"

#include "BoolClasses/BoolVector/boolvector.h"

#include "BoolClasses/BoolInterval/boolinterval.h"

QVector<IntervalFunction> parse(const QString & plaFile);

void generalBonding(QVector<IntervalFunction> & arrayIntFunc);

#endif // FUNCTIONOFMETHOD_H
