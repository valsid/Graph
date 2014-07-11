#ifndef GRAPHSEARCHWORKER_H
#define GRAPHSEARCHWORKER_H

class Edge;
class Vertex;

class GraphSearchWorker
{
public:
    virtual void firstVertex(Vertex *) = 0;
    virtual void nextVertexEdge(Edge *) = 0;
    virtual bool isEnd() = 0;

    virtual void searchEnded() = 0;
};

class DepthSearchWorker : public GraphSearchWorker
{
public:
    virtual void vertexEdgeLeave(Edge *) = 0;
};

class BreadthSearchWorker : public GraphSearchWorker
{
};

#endif // GRAPHSEARCHWORKER_H
