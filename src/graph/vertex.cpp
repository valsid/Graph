#include "vertex.h"
#include <QDebug>

#include "edge.h"
#include <QPainter>
#include <QBitArray>
#include "graph.h"
#include "globalsettings.h"
#include "structures/exceptions.h"


int Vertex::_idCounter = 1;

extern int rand_int(int min, int max);

Vertex::Vertex(const Graph &parentGraph) :
    Vertex(_idCounter++, parentGraph)
{}

Vertex::Vertex(int id, const Graph &parentGraph):
    _parentGraph(parentGraph),
    _graphicsVertex(this)
{
    if(_parentGraph.graphType() == graphTypes::notSelected) {
        throw userException("Select the type of graph, before using graph");
    }

    _id = id;
}

Vertex::~Vertex()
{
    while(!_edges.isEmpty()) {
        Edge *e = _edges.take(_edges.firstKey());
        e->removeLinks();
        delete e;
    }

    while(!_inEdges.isEmpty()) {
        Edge *e = _inEdges.take(_inEdges.firstKey());
        e->removeLinks();
        delete e;
    }

    _graphicsVertex.removeFromScene();

    removeFromGraph();
}

void Vertex::addVertexLink(Vertex *otherVertex, int edgeWeight)
{
    if(otherVertex == nullptr) {
        qFatal("Trying to add nullptr vertex");
    }

    //    _vertices->contains(...);
    for(Edge *edge : outEdges()) {
        if(edge->targetVertex() == otherVertex) {
            qWarning() << "Trying to add an existing link";
            return;
        }
    }

    if(this == otherVertex) {
        Edge *outEdge = new Edge(this, otherVertex, edgeWeight);
        _edges.insert(id(), outEdge);
        _inEdges.insert(id(), outEdge);
        return;
    }

    for(Edge *edge : inEdges()) {
        if(edge->sourceVertex() == otherVertex) {
            _edges.insert(otherVertex->id(), edge->reverseEdge());
            otherVertex->_inEdges.insert(id(), edge->reverseEdge());
            return;
        }
    }

    Edge *outEdge = new Edge(this, otherVertex, edgeWeight);

    _edges.insert(otherVertex->id(), outEdge);
    otherVertex->_inEdges.insert(id(), outEdge);

    if(parentGraph().graphType() == graphTypes::notOrientGraph && !outEdge->isLoop()) {
        Edge *inEdge  = outEdge->reverseEdge();

        otherVertex->_edges.insert(id(), inEdge);
        _inEdges.insert(otherVertex->id(), inEdge);
    }
}

void Vertex::removeVertexLink(Vertex *vertex)
{
    removeInVertexLink(vertex);
    removeOutVertexLink(vertex);
}

void Vertex::removeOutVertexLink(Vertex *vertex)
{
    removeVertexFrom(vertex, _edges);
}

void Vertex::removeInVertexLink(Vertex *vertex)
{
    removeVertexFrom(vertex, _inEdges);
}

void Vertex::removeEdge(Edge *edge)
{
    removeOutEdge(edge);
    removeInEdge(edge);
}

void Vertex::removeOutEdge(Edge *edge)
{
    removeEdgeFrom(edge, _edges);
    removeEdgeFrom(edge, edge->targetVertex()->_inEdges);
}

void Vertex::removeInEdge(Edge *edge)
{
    removeEdgeFrom(edge, _inEdges);
    removeEdgeFrom(edge, edge->sourceVertex()->_edges);
}

void Vertex::removeEdgeFrom(Edge *edge, QMap<int, Edge *> &edges)
{
    QMutableMapIterator<int, Edge *> it(edges);

    while(it.hasNext()) {
        if(it.next().value() == edge) {
            it.remove();
        }
    }
}

void Vertex::removeVertexFrom(Vertex *vertex, QMap<int, Edge *> &edges)
{
    QMutableMapIterator<int, Edge *> i(edges);

    while(i.hasNext()) {
        if(i.next().value()->targetVertex() == vertex) {
            i.remove();
        }
    }
}
/*
void GraphVertex::setPen(const QPen pen)
{
    QAbstractGraphicsShapeItem::setPen(pen);
}
*/

void Vertex::resetIdCounter()
{
    _idCounter = 1;
}

void Vertex::removeFromGraph()
{
    for(Edge *e : outEdges()) {
        e->removeLinks();
    }

    for(Edge *e : inEdges()) {
        e->removeLinks();
    }
}

void Vertex::printDump()
{
    qDebug() << "Vertex id: " << id() << "\nOut edges: ";
    for(Edge *e : outEdges()){
        qDebug() << "\tid: " << e->id() << "target: " << e->targetVertex()->id();
    }

    qDebug() << "In edges: ";
    for(Edge *e : inEdges()) {
        qDebug() << "\tid: " << e->id() << "source: " << e->sourceVertex()->id();
    }
}

const GraphicVertex *Vertex::graphicsVertex() const
{
    return &_graphicsVertex;
}

QList<Edge *> Vertex::outEdges() const
{
    return _edges.values();
}

QList<Edge *> Vertex::inEdges() const
{
    return _inEdges.values();
}

int Vertex::id() const
{
    return _id;
}

const Graph &Vertex::parentGraph()
{
    return _parentGraph;
}

GraphicVertex *Vertex::graphicsVertex()
{
    return &_graphicsVertex;
}
