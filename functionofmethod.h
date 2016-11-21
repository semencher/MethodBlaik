#ifndef FUNCTIONOFMETHOD_H
#define FUNCTIONOFMETHOD_H

#include "global.h"

#include "BoolClasses/BoolInterval/boolinterval.h"

void parse(const QString & plaFile, QVector<IntervalFunction *> & arrayIntFunc,
           QVector<IntervalFunction *> & a0);

void generalBonding(QVector<IntervalFunction *> & arrayIntFunc);

void absorb(QVector<IntervalFunction *> & arrayIntFunc);

void correction(QVector<IntervalFunction *> & a1, QVector<IntervalFunction *> & a0);

void show(QVector<IntervalFunction *> & a1, QVector<IntervalFunction *> & a0, std::string filename);

#endif // FUNCTIONOFMETHOD_H
