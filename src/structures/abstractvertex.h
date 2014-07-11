#ifndef ABSTRACTVERTEX_H
#define ABSTRACTVERTEX_H

// TODO: make abstract

#include "abstractedge.h"

#include <QList>

class AbstractVertex
{
public:
    virtual int id() const = 0;

    virtual void addVertexLink(AbstractVertex *otherVertex, int edgeWeight) = 0;
    virtual void removeVertexLink(AbstractVertex *vertex) = 0;

    virtual QList<AbstractEdge *> outEdges() const = 0;
    virtual QList<AbstractEdge *> inEdges() const = 0;
};

#endif // ABSTRACTVERTEX_H
