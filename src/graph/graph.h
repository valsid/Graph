#ifndef GRAPH_H
#define GRAPH_H

#include "vertex.h"
#include "edge.h"

#include "../structures/graphsearchworker.h"
#include "../structures/graphTypes.h"
#include "../algorithm/algorithms.h"

#include <QTimer>
#include <QSet>

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(graphTypes type = graphTypes::orientGraph, QObject *parent = 0);
    ~Graph();

    Vertex *createNewVertex(const QVector<edgeData> * outVertexID = nullptr, QPointF pos = QPoint(0, 0), bool fixedPos = false);
    void removeVertex(int id);
    void removeVertex(Vertex *vertex);

    QList<Vertex *> allVertices() const;

    void calculatePos();

    Graph* getCopy(QObject *newGraphParent = 0);
    Graph* getTransposeGraphCopy();
    QSet<Edge *> allEdges() const;
    QString toTextGraph();

    graphTypes graphType() const;

// Algorithms
    // graphMatrix defined in algorithms.h
    graphMatrix *incidenceMatrix()        const;
    graphMatrix *adjacencyMatrix()        const;
    graphMatrix *distanceMatrix()         const;
    graphMatrix *reachabilityMatrix()     const;
    graphMatrix *weightedDistanceMatrix() const;

    QVector<degree> *vertexDegree()   const;
    QVector<int>    *hangingVertex()  const;
    QVector<int>    *isolatedVertex() const;

    graphCycles *checkCycles() const;

    void breadthSearch(int beginVertexId, DepthSearchWorker *worker) const;
    void depthSearch  (int beginVertexId, DepthSearchWorker *worker) const;

    graphConnectivity connectivity() const;
    QList<int> topologicalSorting()  const;

    pathToAll minPathsDijkstra   (int beginVertexId) const;
    pathToAll minPathsBellmanFord(int beginVertexId) const;

    bool isPlanarity() const;
    QMap<int, int> coloring() const;

    flow maxFlowFordFulkerson(int startVertexId, int endVertexId);

// End algorithms

    void highlightVertex(const QList<Vertex *> &order, int delay = 500);

    Vertex *getVertex(int id) const;
    bool isExistVertex(int id) const;

signals:
    void hightLightEnded();

public slots:
    void forTest() {
//        inverseGraph();
    }

    void clearGraph();
    void setAllVertexDefaultColor();
    void inverseGraph();

private slots:
    void highlightNextVertex();

private:
    graphTypes _graphType;

    QList<Vertex *> _highlightList;
    QTimer _highlightVertexTimer;

    QMap<int, Vertex*> _vertices;
};

#endif // GRAPH_H
