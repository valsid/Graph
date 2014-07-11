#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QObject>
#include <QVector>
#include <QSet>
#include "../structures/graphTypes.h"
#include "../graph/edge.h"
#include "../structures/graphsearchworker.h"

const int INF = 1 << 30; // 2^30 // 1073741824
typedef QList</*const */Vertex *> vertexList;

// enum class conformity {noneLink = 0, haveLink = 1, inLink = 3, outLink = 5, loopLink = 7}; // binary 000, 001, 011, 101, 111

enum class graphConnectivity {notConnectivity, STRONGLY, WEAKLY, SIDED};

struct degree {
    int vertexID;
    int vertexDegree;
    int inVertexDegree = 0;
};

enum class matrixType {none, IncidenceMatrix, AdjacencyMatrix, DistanceMatrix, ReachabilityMatrix, FlowWeightsMatrix};

typedef QMap<int, QMap<int, int> > simpleMatrix;
struct graphMatrix
{
    QVector<int> rowKeys;
    QVector<int> colKeys;
    simpleMatrix matrix;
    matrixType   type;
};

typedef QList<const Vertex *> graphPath;
struct graphCycles
{
    QVector<QVector<int>> cyclesVertexId;
    QVector<graphPath> allCycles;
};

struct weightedPath {
    weightedPath() { }

    weightedPath(bool valid, int w, QVector<int> p) :
        isValid(valid),
        weight(w),
        path(p)
    {}

    bool isValid = false;
    int  weight  = -1;
    QVector<int> path;
};

struct pathToAll {
    bool isHaveNegativeCycles;
    bool isHaveNegativeEdgeWeight;
    int fromVertexId;
    QVector<int> keys;
    QMap<int, weightedPath> paths;
};

struct flow {
    int value         =  0;
    int beginVertexId = -1;
    int endVertexId   = -1;
    QSet<int> residualNetwork;
    graphMatrix resultFlowMatrix;
};

struct Node {
    int  prev        = -1;
    int  flowReserve = -1;
    bool isDefined   = false;
};

// Algorithms is provided "as is"

class Algorithms : public QObject
{
    Q_OBJECT
public:
    explicit Algorithms(QObject *parent = 0);

    static graphMatrix *getIncidenceMatrix     (vertexList verticesList, graphTypes graphType);
    static graphMatrix *getAdjacencyMatrix     (vertexList verticesList, bool weighted = false);
    static graphMatrix *getDistanceMatrix      (vertexList verticesList, bool weighted = false);
    static graphMatrix *getReachabilityMatrix  (vertexList verticesList);
    static graphMatrix *getDistanceMatrix      (graphMatrix *weightedAdjacencyMatrix);

    static QVector<degree> *getVerticesDegree(vertexList verticesList, graphTypes graphType);
    static QVector<int>    *getHangingVertex (vertexList verticesList, graphTypes graphType);
    static QVector<int>    *getIsolatedVertex(vertexList verticesList, graphTypes graphType);

    static void breadthSearch(Vertex *firstVertex, GraphSearchWorker *worker);  // vertexList for erase token
    static void depthSearch  (Vertex *firstVertex, DepthSearchWorker *worker);

    static graphCycles *checkCycles(vertexList verticesList);
    static graphConnectivity checkConnectivity(vertexList verticesList, graphTypes graphType);

    static QList<int> topologicalSorting(vertexList verticesList, graphTypes graphType);

    static bool isHaveNegativeWeight(vertexList verticesList);
    static bool isHaveNegativeCyclesFrom(int startVertexId, vertexList verticesList);

    static pathToAll BellmanFordAlgor(int startVertexId, vertexList verticesList);

    static pathToAll getMinPathsDijkstra(int startVertexId, vertexList vList);
    static weightedPath getMinPathsDijkstra(int startVertexId, int endVertexId, vertexList vList);

    static bool isPlanarity(vertexList verticesList);
    static QMap<int, int> coloring(vertexList vertexList);

    static flow FordFulkerson(int startVertexId, int endVertexId, vertexList verticesList);
    static graphMatrix *FordFulkersonAllPair(graphMatrix flowMatrix);

signals:

public slots:

private:
    static graphMatrix _encodeMatrixToSandardIndex(const graphMatrix &matrix);
    static graphMatrix _decodeMatrix(const graphMatrix &fromMatrix, QVector<int> rowKeys, QVector<int> colKeys);

    static int findPath(graphMatrix &flowWeightMatrix, int current, int end, graphMatrix &currentFlowWeight, QSet<int> *visibled = nullptr, int MaxPossibleFlow = INF);

    static weightedPath tempPath;
    static bool isShowPaths;
    static void checkContainsVertex(int beginVertexId, int endVertexId, vertexList vertexList);

    static bool checkStronglyConnectivity(vertexList verticesList, graphTypes graphType);
    static bool checkWeaklyConnectivity  (vertexList verticesList);
    static bool checkSidedConnectivity   (vertexList verticesList, graphTypes graphType);

    static void depthSearchRecursive(Edge * edge, DepthSearchWorker *worker, QSet<int> *visitedVertexId);

    static void BellmanFordPathRecovery(weightedPath *resultPath, int endId, const QMap<int, int> &path);
};

#endif // ALGORITHMS_H
