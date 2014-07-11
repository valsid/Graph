#include "getgraphvertexleavelistworker.h"




graphTopologicalSortWorker::graphTopologicalSortWorker()
{

}

void graphTopologicalSortWorker::firstVertex(Vertex *vertex)
{
    firstId = vertex->id();
}

void graphTopologicalSortWorker::nextVertexEdge(Edge *edge)
{
    Q_UNUSED(edge);
}

void graphTopologicalSortWorker::vertexEdgeLeave(Edge *edge)
{
    if(_vertexIdList.contains(edge->targetVertex()->id())) {
        return;
    }
    _vertexIdList.append(edge->targetVertex()->id());
}

bool graphTopologicalSortWorker::isEnd()
{
    return false;
}

void graphTopologicalSortWorker::searchEnded()
{
    if(_vertexIdList.contains(firstId)) {
        return;
    }
    _vertexIdList.append(firstId);
}

QList<int> graphTopologicalSortWorker::leaveList() const
{
    return _vertexIdList;
}
