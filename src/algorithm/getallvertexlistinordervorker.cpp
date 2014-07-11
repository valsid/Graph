#include "getallvertexlistinordervorker.h"

void getVertexSearchOrderWorker::firstVertex(Vertex *vertex)
{
    _vertexList.append(vertex);
}

void getVertexSearchOrderWorker::nextVertexEdge(Edge *edge)
{
    _vertexList.append(edge->targetVertex());
}

void getVertexSearchOrderWorker::vertexEdgeLeave(Edge *)
{

}

bool getVertexSearchOrderWorker::isEnd()
{
    return false;
}

void getVertexSearchOrderWorker::searchEnded()
{

}

QVector<Vertex *> getVertexSearchOrderWorker::allItem() const
{
    return _vertexList;
}

