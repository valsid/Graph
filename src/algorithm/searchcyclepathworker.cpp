#include "searchcyclepathworker.h"

searchPathWorker::searchPathWorker():
    searchPathWorker(nullptr)
{

}

searchPathWorker::searchPathWorker(const Vertex *endVertex):
    _searchPathVertex(endVertex)
{

}

void searchPathWorker::firstVertex(Vertex *fVertex)
{
    _currentPath.append(fVertex);
}

void searchPathWorker::nextVertexEdge(Edge *edge)
{
    _currentPath.append(edge->targetVertex());
    for(Edge *edg : edge->targetVertex()->outEdges()) {
        if(edg->targetVertex() == _searchPathVertex) {
            _allPathArr.append(_currentPath);
            _allPathArr.last().append(edg->targetVertex());
        }
    }
}

void searchPathWorker::vertexEdgeLeave(Edge *)
{
    _currentPath.removeLast();
}

bool searchPathWorker::isEnd()
{
    return false;
}

void searchPathWorker::searchEnded()
{

}

QVector<QList<const Vertex *> > searchPathWorker::getAllPath()
{
    return _allPathArr;
}

void searchPathWorker::reset(const Vertex* endVertex)
{
    _searchPathVertex = endVertex;
    _currentPath.clear();
    _allPathArr.clear();
}
