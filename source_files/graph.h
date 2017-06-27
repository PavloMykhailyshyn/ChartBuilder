#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

class Graph
{
public:
    Graph();
    QPair <QVector<double>, QVector<double> > BuildGraphByFormula(const QString& formula);
    ~Graph();
};

#endif // GRAPH_H
