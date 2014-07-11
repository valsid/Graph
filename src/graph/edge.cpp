#include "edge.h"
#include <QPainter>
#include <QtMath>

#include "graph.h"
#include "../globalsettings.h"

int Edge::_idCounter = 1;

extern double normal_rand_number(double mean, double stddev);

Edge::Edge(Vertex *sourceNode, Vertex *targetNode, int weight) :
    _vertex(targetNode),
    _weight(weight),
    _isLinked(true),
    _isPrepareToDelete(false),
    _graphicEdge( new GraphicEdge(*this)),
    eType(edgeType::mainEdge)
{
    _id = _idCounter++;
    _isLoop = (sourceNode == targetNode);
    _reverseEdge = new Edge(sourceNode, this, _graphicEdge, weight, _id, _isLoop);
}

Edge::~Edge()
{
    _isPrepareToDelete = true;

    removeLinks();

    _graphicEdge->removeFromScene();

    if(!reverseEdge()->_isPrepareToDelete) {
        delete _reverseEdge;
    }
}

Edge::Edge(Vertex *targetNode, Edge *reverseEdge, QSharedPointer<GraphicEdge> graphicEdge, int weight, int id,  bool isLoop) :
    _vertex(targetNode),
    _reverseEdge(reverseEdge),
    _weight(weight),
    _id(id),
    _isLoop(isLoop),
    _isLinked(true),
    _graphicEdge(graphicEdge),
    eType(edgeType::additionalEdge)
{ }

void Edge::setWeight(int weight)
{
    _weight = weight;
}

Edge *Edge::reverseEdge() const
{
    return _reverseEdge;
}

int Edge::weight() const
{
    return _weight;
}

Vertex *Edge::targetVertex() const
{
    return _vertex;
}

Vertex *Edge::sourceVertex() const
{
    return reverseEdge()->targetVertex();
}

int Edge::id() const
{
    return _id;
}

void Edge::swapDirection()    // its friend method to vertex
{
    if(targetVertex()->parentGraph().graphType() == graphTypes::notOrientGraph) {
        return;
    }

    Vertex *newSource = targetVertex();
    Vertex *newTarget = sourceVertex();

    removeLinks();

    // it private
    newSource->_edges.insert(newTarget->id(), this);
    newTarget->_inEdges.insert(newSource->id(), this);

    std::swap(_vertex, reverseEdge()->_vertex);

    graphicEdge()->updatePos();
    graphicEdge()->update();
}

void Edge::removeLinks()
{
    if(_isLinked) {
        targetVertex()->removeEdge(this);
        sourceVertex()->removeEdge(this);
        _isLinked = false;
    }

    if(reverseEdge()->_isLinked) {
        targetVertex()->removeEdge(reverseEdge());
        sourceVertex()->removeEdge(reverseEdge());
        _reverseEdge->_isLinked = false;
    }
}

GraphicEdge *Edge::graphicEdge() const
{
    return _graphicEdge.data();
}

bool Edge::isLoop() const
{
    return _isLoop;
}
