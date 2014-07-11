#ifndef GETGRAPHVERTEXLEAVELISTWORKER_H
#define GETGRAPHVERTEXLEAVELISTWORKER_H

#include "../structures/graphsearchworker.h"
#include "../graph/vertex.h"

class graphTopologicalSortWorker : public DepthSearchWorker
{
public:
    graphTopologicalSortWorker();

    virtual void firstVertex(Vertex *vertex);
    virtual void nextVertexEdge(Edge *edge);
    virtual void vertexEdgeLeave(Edge *edge);
    virtual bool isEnd();
    virtual void searchEnded();

    QList<int> leaveList() const;

private:
    int firstId = 0;
    QList<int> _vertexIdList;
};

#endif // GETGRAPHVERTEXLEAVELISTWORKER_H
