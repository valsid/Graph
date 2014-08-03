#include "graph.h"

#include "../globalsettings.h"
#include "structures/comparators.h"

#include <QMessageBox>
#include <QDebug>

Graph::Graph(graphTypes type, QObject *parent) :
    QObject(parent),
    _graphType(type)
{
}

Graph::~Graph()
{
    clearGraph();
}

Vertex *Graph::createNewVertex(const QVector<edgeData> *outVertexID, QPointF pos, bool isFixedPos)
{
    Vertex *newNode = new Vertex(*this);
    if(isFixedPos) {
        newNode->graphicsVertex()->setFixedPos(isFixedPos);
        newNode->graphicsVertex()->setPos(pos);
    }
    _vertices.insert(newNode->id(), newNode);


    if(outVertexID != nullptr) {
        for(edgeData data : *outVertexID) {
            auto vertexFromId = getVertex(data.vertexId);
            if(vertexFromId == nullptr) {
                continue;
            }

            if(data.direction == directions::OUTPUT) {
                newNode->addVertexLink(vertexFromId, data.edgeWeight);
            } else {
                vertexFromId->addVertexLink(newNode, data.edgeWeight);
            }
        }
    }

//    if(inVertexID != nullptr) {
//        for(edgeData data : *inVertexID) {
//            auto vertexFromID = searchVertex(data.vertexId);
//            if(vertexFromID == nullptr) {
//                continue;
//            }

//            vertexFromID->addVertexLink(newNode, data.vertexWeight);
//        }
//    }


    return newNode;
}

void Graph::removeVertex(int id)
{
    removeVertex(getVertex(id));
}

void Graph::removeVertex(Vertex *vertex)
{
    if(vertex != nullptr && _vertices.contains(vertex->id())) {
//        qDebug() << "rem vertex" << vertex->id();
        delete _vertices.take(vertex->id());
    } else {
        // TODO: make warning window
        qWarning() << "Remove nullptr vertex from graph";
    }
}

void Graph::clearGraph()
{
    while(!_vertices.isEmpty()) {
        removeVertex(_vertices.firstKey());
    }
    Vertex::resetIdCounter();
}

void Graph::setAllVertexDefaultColor()
{
    for(Vertex *vertex : allVertices()) {
        vertex->graphicsVertex()->restoreDefaultColor();
    }
}

QList<Vertex *> Graph::allVertices() const
{
    return _vertices.values();
}

Graph *Graph::getCopy(QObject *newGraphParent)
{
    Graph *copy = new Graph(_graphType, newGraphParent);

    for(Vertex *vertex : allVertices()) {
        copy->_vertices[vertex->id()] = new Vertex(vertex->id(), *copy);
    }

    for(Vertex *vertex : allVertices()) {
        for(Edge *edge : vertex->outEdges()) {
            copy->_vertices[vertex->id()]->addVertexLink( copy->_vertices[edge->targetVertex()->id()], edge->weight() );
        }
    }

    return copy;
}

Graph *Graph::getTransposeGraphCopy()
{
    Graph *g = getCopy();
    g->inverseGraph();
    return g;
}

QSet<Edge *> Graph::allEdges() const
{
    QSet<Edge *> edgeSet;

    for(Vertex *v : allVertices()) {
        for(Edge *e : v->outEdges()) {
            edgeSet.insert(e);
        }
    }

    return edgeSet;
}

QString Graph::toTextGraph()
{
    QString result;
    result.append("w\r\n");

    if(_vertices.isEmpty()) {
        result.append("0 0");
    } else {
        int maxId = _vertices.first()->id();
        for(Vertex *v : allVertices()) {
            if(maxId < v->id()) {
                maxId = v->id();
            }
        }

        QSet<Edge*> edges = allEdges();
        result.append(QString::number(maxId) + " " + QString::number(edges.size()) + "\r\n");
        QSet<int> writedLoopsVertexId;
        for(Edge *e : edges) {
            int sourceId = e->sourceVertex()->id();
            int targetId = e->targetVertex()->id();
            if(sourceId == targetId) {
                if(writedLoopsVertexId.contains(targetId)) {
//                    continue;
                } else {
                    writedLoopsVertexId.insert(targetId);
                }
            }
            result.append(QString::number(e->sourceVertex()->id()) + " " + QString::number(e->targetVertex()->id()) + ' ' + QString::number(e->weight()) + "\n");
        }
    }

    return result;
}

graphTypes Graph::graphType() const
{
    return _graphType;
}

Vertex *Graph::getVertex(int id) const
{
    return _vertices.value(id, nullptr);
}

bool Graph::isExistVertex(int id) const
{
    return _vertices.contains(id);
//    return getVertex(id) != nullptr;
}

graphMatrix *Graph::incidenceMatrix() const
{
    return Algorithms::getIncidenceMatrix(allVertices(), _graphType);
}

graphMatrix *Graph::adjacencyMatrix() const
{
    return Algorithms::getAdjacencyMatrix(allVertices());
}

graphMatrix *Graph::distanceMatrix() const
{
    return Algorithms::getDistanceMatrix(allVertices());
}

graphMatrix *Graph::reachabilityMatrix() const
{
    return Algorithms::getReachabilityMatrix(allVertices());
}

graphMatrix *Graph::weightedDistanceMatrix() const
{
    return Algorithms::getDistanceMatrix(allVertices(), true);
}

QVector<degree> *Graph::vertexDegree() const
{
    return Algorithms::getVerticesDegree(allVertices(), _graphType);
}

QVector<int> *Graph::hangingVertex() const
{
    return Algorithms::getHangingVertex(allVertices(), _graphType);
}

QVector<int> *Graph::isolatedVertex() const
{
    return Algorithms::getIsolatedVertex(allVertices(), _graphType);
}

graphCycles *Graph::checkCycles() const
{
    return Algorithms::checkCycles(allVertices());
}

void Graph::breadthSearch(int beginVertexId, DepthSearchWorker *worker) const
{
    auto tmp = getVertex(beginVertexId);
    Algorithms::breadthSearch(tmp, worker);
}

void Graph::depthSearch(int beginVertexId, DepthSearchWorker *worker) const
{
    auto tmp = getVertex(beginVertexId);
    Algorithms::depthSearch(tmp, worker);
}

graphConnectivity Graph::connectivity() const
{
    return Algorithms::checkConnectivity(allVertices(), _graphType);
}

QList<int> Graph::topologicalSorting() const
{
    return Algorithms::topologicalSorting(allVertices(), _graphType);
}

pathToAll Graph::minPathsDijkstra(int beginVertexId) const
{
    return Algorithms::getMinPathsDijkstra(beginVertexId, _vertices.values());
}

pathToAll Graph::minPathsBellmanFord(int beginVertexId) const
{
    return Algorithms::BellmanFordAlgor(beginVertexId, allVertices());
}

bool Graph::isPlanarity() const
{
    return Algorithms::isPlanarity(allVertices());
}

QMap<int, int> Graph::coloring() const
{
    return Algorithms::coloring(allVertices());
}

flow Graph::maxFlowFordFulkerson(int startVertexId, int endVertexId)
{
    return Algorithms::FordFulkerson(startVertexId, endVertexId, allVertices());
}

void Graph::highlightVertex(const QList<Vertex *> &order, int delay)
{
    if(_highlightVertexTimer.isActive()) {
        _highlightVertexTimer.stop();
        setAllVertexDefaultColor();
    }

    _highlightList = order;
    connect(&_highlightVertexTimer, &QTimer::timeout,
            this,                   &Graph::highlightNextVertex);

    _highlightVertexTimer.start(delay);
}

void Graph::highlightNextVertex()
{
    if(_highlightList.empty()) {
        _highlightVertexTimer.stop();
        disconnect(&_highlightVertexTimer, &QTimer::timeout,
                this, &Graph::highlightNextVertex);
        setAllVertexDefaultColor();

        emit hightLightEnded();
    } else {
        _highlightList.takeFirst()->graphicsVertex()->setPen( GlobalSettings::highlightVertexPen() );
    }
}

void Graph::inverseGraph()
{
    if(graphType() == graphTypes::orientGraph) {
        for(Edge *e : allEdges()) {
            e->swapDirection();
        }
    }
}
