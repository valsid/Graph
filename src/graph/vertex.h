#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QDebug>

#include "edge.h"
#include "graphicvertex.h"
#include "../structures/graphTypes.h"
class Graph;

class Vertex
{
    friend class Graph;     // for make graph copy
    friend void Edge::swapDirection();

    explicit Vertex(int id, const Graph &parentGraph);

public:
    explicit Vertex(const Graph &parentGraph);
    ~Vertex();

    void addVertexLink(Vertex *otherVertex, int edgeWeight = 0);
    void removeVertexLink(Vertex *vertex);
    void removeEdge(Edge *edge);
    QList<Edge *> outEdges() const;
    QList<Edge *> inEdges() const;

    int id() const;
    const Graph &parentGraph();
    GraphicVertex *graphicsVertex();
    const GraphicVertex *graphicsVertex() const;
    static void resetIdCounter();

    void removeFromGraph();

    void printDump();

private:
    Q_DISABLE_COPY(Vertex)

    void removeOutVertexLink(Vertex *vertex);
    void removeInVertexLink(Vertex *vertex);
    void removeOutEdge(Edge *edge);
    void removeInEdge(Edge *edge);

    static void removeEdgeFrom(Edge *edge, QMap<int, Edge *> &outEdges);
    static void removeVertexFrom(Vertex *vertex, QMap<int, Edge *> &outEdges);

    QMap<int, Edge *> _edges;
    QMap<int, Edge *> _inEdges;
    const Graph &_parentGraph;
    GraphicVertex _graphicsVertex;
    int _id;

    static int _idCounter;
};

#endif // GRAPHNODE_H
