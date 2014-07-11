#ifndef GETALLVERTEXLISTINORDERVORKER_H
#define GETALLVERTEXLISTINORDERVORKER_H

#include "../structures/graphsearchworker.h"
#include "../graph/vertex.h"

class getVertexSearchOrderWorker : public DepthSearchWorker, public BreadthSearchWorker
{
public:
    virtual void firstVertex(Vertex *vertex);
    virtual void nextVertexEdge(Edge *edge);
    virtual void vertexEdgeLeave(Edge *);
    virtual bool isEnd();
    virtual void searchEnded();

    QVector<Vertex *> allItem() const;

private:
    QVector<Vertex *> _vertexList;
};

#endif // GETALLVERTEXLISTINORDERVORKER_H
