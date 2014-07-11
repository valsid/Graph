#ifndef SEARCHCYCLEPATHWORKER_H
#define SEARCHCYCLEPATHWORKER_H

#include "../structures/graphsearchworker.h"
#include "../graph/vertex.h"

    // Знаходяться не всі цикли, за умовою завдання це дозволяється
class searchPathWorker : public DepthSearchWorker
{
public:
    searchPathWorker();
    searchPathWorker(const Vertex *endVertex);

    virtual void firstVertex(Vertex *fVertex);
    virtual void nextVertexEdge(Edge *edge);
    virtual void vertexEdgeLeave(Edge *leaveVertexEdge);
    virtual bool isEnd();
    virtual void searchEnded();

    QVector< QList<const Vertex*> > getAllPath();
    void reset(const Vertex *endVertex);

private:
    const Vertex *_searchPathVertex;

    QList<const Vertex*> _currentPath;
    QVector< QList<const Vertex*> > _allPathArr;
};

#endif // SEARCHCYCLEPATHWORKER_H
