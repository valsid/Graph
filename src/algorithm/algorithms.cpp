#include "algorithms.h"
#include <utility>
using std::min;
using std::max;
#include <QDebug>

#include "../graph/edge.h"
#include "../graph/vertex.h"
#include <QStack>
#include <QQueue>

#include "searchcyclepathworker.h"
#include "getgraphvertexleavelistworker.h"

weightedPath Algorithms::tempPath =  weightedPath();
bool Algorithms::isShowPaths = true;


Algorithms::Algorithms(QObject *parent) :
    QObject(parent)
{
}

graphMatrix *Algorithms::getIncidenceMatrix(vertexList verticesList, graphTypes graphType)
{
    graphMatrix *resultMatrix = new graphMatrix;

    resultMatrix->rowKeys.reserve(verticesList.size());
    resultMatrix->colKeys.reserve(verticesList.size());
    resultMatrix->type   = matrixType::IncidenceMatrix;


    int vId, // vertex ID
        rId; // reverse vertex ID

    for(const Vertex *vertex : verticesList) {
        resultMatrix->rowKeys.append(vertex->id());
        for(Edge *edge : vertex->outEdges()) {
            resultMatrix->colKeys.append(edge->id());

            vId = edge->targetVertex()->id();
            rId = edge->sourceVertex()->id();

            if(edge->targetVertex() == edge->sourceVertex()) {
                resultMatrix->matrix[vId][edge->id()] = 2;
                continue;
            }
            resultMatrix->matrix[vId][edge->id()] = 1;
            resultMatrix->matrix[rId][edge->id()]
                    = (graphType == graphTypes::orientGraph) ? -1 : 1;
        }

    }

    for(int row : resultMatrix->rowKeys) {
        for(int col : resultMatrix->colKeys) {
            if(!resultMatrix->matrix[row].contains(col)) {
                resultMatrix->matrix[row][col] = 0;
            }
        }
    }

    return resultMatrix;
}

graphMatrix *Algorithms::getAdjacencyMatrix(vertexList verticesList, bool weighted)
{
    graphMatrix *resultMatrix = new graphMatrix;

    resultMatrix->rowKeys.reserve(verticesList.size());
    resultMatrix->colKeys.reserve(verticesList.size());
    resultMatrix->type     = matrixType::AdjacencyMatrix;

    for(const Vertex *i : verticesList) {
        resultMatrix->rowKeys.append(i->id());
        resultMatrix->colKeys.append(i->id()); // its square matrix
        for (Edge *edge : i->outEdges()) {
            if(weighted) {
                resultMatrix->matrix[i->id()][edge->targetVertex()->id()] = edge->weight();
            } else {
                resultMatrix->matrix[i->id()][edge->targetVertex()->id()] = 1;
            }
        }
    }

    return resultMatrix;
}

graphMatrix *Algorithms::getDistanceMatrix(graphMatrix *weightedAdjacencyMatrix)
{
    int size = weightedAdjacencyMatrix->rowKeys.size();
    if(size != weightedAdjacencyMatrix->colKeys.size() || weightedAdjacencyMatrix->type != matrixType::AdjacencyMatrix) {
        delete weightedAdjacencyMatrix;
        qFatal("Bad matrix type");
    }

    simpleMatrix &matrix = weightedAdjacencyMatrix->matrix;
    weightedAdjacencyMatrix->type   = matrixType::DistanceMatrix;

    for(int i : weightedAdjacencyMatrix->rowKeys) {
        for(int j : weightedAdjacencyMatrix->colKeys) {
            if(i != j && (weightedAdjacencyMatrix->matrix)[i][j] == 0) {
                (weightedAdjacencyMatrix->matrix)[i][j] = INF;
            }
        }
    }

    for(int k : weightedAdjacencyMatrix->rowKeys) {
        for(int i : weightedAdjacencyMatrix->rowKeys) {
            for(int j : weightedAdjacencyMatrix->colKeys) {
                if(matrix[i][k] != INF && matrix[k][j] != INF) {
                    matrix[i][j] = min(int(matrix[i][j]), matrix[i][k] + matrix[k][j]);
                }
            }
        }
    }


    for(int i : weightedAdjacencyMatrix->rowKeys) {
        for(int j : weightedAdjacencyMatrix->colKeys) {
            if(matrix[i].contains(j)) {
                if(matrix[i][j] == INF) {
                    matrix[i].remove(j);
                }
            }
        }
    }

    return weightedAdjacencyMatrix;
}

graphMatrix *Algorithms::getDistanceMatrix(vertexList verticesList, bool weighted)
{
    return getDistanceMatrix(getAdjacencyMatrix(verticesList, weighted));
}

graphMatrix *Algorithms::getReachabilityMatrix(vertexList verticesList)
{

    graphMatrix* newMatrix = getAdjacencyMatrix(verticesList);
//    int size = newMatrix->rowKeys.size();
//    if(size != newMatrix->colKeys.size()) {
//        ;
//    }

    simpleMatrix &matrix = newMatrix->matrix;
    newMatrix->type   = matrixType::ReachabilityMatrix;

    for(int k : newMatrix->rowKeys) {
        for(int i : newMatrix->rowKeys) {
            for(int j : newMatrix->colKeys) {
                matrix[i][j] = int(matrix[i][j]) | (matrix[i][k] & matrix[k][j]);
            }
        }
    }

    for(int i : newMatrix->rowKeys) {
        matrix[i][i] = 1;
    }

    return newMatrix;
}

QVector<degree> *Algorithms::getVerticesDegree(vertexList verticesList, graphTypes graphType)
{
    auto verticesDegree = new QVector<degree>();
    degree tmp;
    for(const Vertex *vertex : verticesList) {
        tmp.vertexID = vertex->id();
        tmp.vertexDegree = vertex->outEdges().size();
        if(graphType == graphTypes::orientGraph) {
            tmp.inVertexDegree = vertex->inEdges().size();
            tmp.vertexDegree += tmp.inVertexDegree;
        }
        verticesDegree->append(tmp);
    }
    return verticesDegree;
}

QVector<int> *Algorithms::getHangingVertex(vertexList verticesList, graphTypes graphType)
{
    QVector<degree> *verticesDegree = getVerticesDegree(verticesList, graphType);
    QVector<int>    *resultVector   = new QVector<int>();

    for(degree d : *verticesDegree) {
        if(d.vertexDegree == 1) {
            resultVector->append(d.vertexID);
        }
    }

    return resultVector;
}

QVector<int> *Algorithms::getIsolatedVertex(vertexList verticesList, graphTypes graphType)
{
    QVector<degree> *verticesDegree = getVerticesDegree(verticesList, graphType);
    QVector<int>    *resultVector   = new QVector<int>();

    for(degree d : *verticesDegree) {
        if(d.vertexDegree == 0) {
            resultVector->append(d.vertexID);
        }
    }

    return resultVector;
}


void Algorithms::breadthSearch(Vertex * firstVertex, GraphSearchWorker *worker)
{
    if(firstVertex == nullptr || worker == nullptr) {
        return;
    }

    QQueue<Edge *> edgeQueue;
    QSet<int> visitedVertexId;
    Edge * tmpEdge;

    worker->firstVertex(firstVertex);
    visitedVertexId.insert(firstVertex->id());
//    firstVertex->setBitToken(0, true);
    for(Edge *edge : firstVertex->outEdges()) {
//        qDebug() << "111";
        edgeQueue.append(edge);
    }

    while( !edgeQueue.empty() ) {
        tmpEdge = edgeQueue.takeFirst();
//        qDebug() << tmpEdge->vertex()->id();
        if(visitedVertexId.contains(tmpEdge->targetVertex()->id())) {
//            qDebug() << "if" << tmpEdge->vertex()->id() << edgeQueue.empty();
//        if(tmpEdge->vertex()->bitToken(0) == true) {
            continue;
        }
        worker->nextVertexEdge(tmpEdge);

        if(worker->isEnd()) {
            break;
        }

        visitedVertexId.insert(tmpEdge->targetVertex()->id());

        for(Edge * edge : tmpEdge->targetVertex()->outEdges()) {
            edgeQueue.append(edge);
        }
    }

    worker->searchEnded();
}

/*
void Algorithms::depthSearch(const Vertex *firstVertex, DepthSearchWorker *worker)
{
    if(firstVertex == nullptr || worker == nullptr) {
        return;
    }
    QSet<int> visitedVertexId;
    QStack<Edge *> edgeStack;
    Edge * tmpEdge;

    worker->firstVertex(firstVertex);
    visitedVertexId.insert(firstVertex->id());

    bool haveVisitedVertex = false;

//    edgeStack.push(firstVertex->edges()->at(0));

    do {
        tmpEdge = edgeStack.top();

        for()


    } while(!edgeStack.empty());
}
*/

void Algorithms::depthSearch(Vertex * firstVertex, DepthSearchWorker *worker)
{
    if(firstVertex == nullptr || worker == nullptr) {
        // TODO: make exception?
        return;
    }

    QSet<int> visitedVertexId;

    worker->firstVertex(firstVertex);
    visitedVertexId.insert(firstVertex->id());

    for(Edge * tmpEdge : firstVertex->outEdges()) {
        depthSearchRecursive(tmpEdge, worker, &visitedVertexId);
    }

    worker->searchEnded();
}

void Algorithms::depthSearchRecursive(Edge * edge, DepthSearchWorker *worker, QSet<int> *visitedVertexId)
{
    if(visitedVertexId->contains(edge->targetVertex()->id())) {
        return;
    }

    worker->nextVertexEdge(edge);
    if(worker->isEnd()) {
        return;
    }

    visitedVertexId->insert(edge->targetVertex()->id());

    for(Edge * tmpEdge : edge->targetVertex()->outEdges()) {
        depthSearchRecursive(tmpEdge, worker, visitedVertexId);
        if(worker->isEnd()) {
            break;
        }
    }
    worker->vertexEdgeLeave(edge);
}

// Знаходяться не всі цикли, за умовою завдання це дозволяється
graphCycles *Algorithms::checkCycles(vertexList verticesList)
{
    graphCycles *cycles = new graphCycles();
    searchPathWorker w;

    QVector<graphPath> tmpPath;
    QVector<int> pathId;

    for(Vertex * vertex: verticesList) {
        w.reset(vertex);
        depthSearch(vertex, &w);
        tmpPath = w.getAllPath();
        for(graphPath path : tmpPath) {
            cycles->allCycles.append(path);
            for(const Vertex * vertex: path) {
                pathId.append(vertex->id());
            }
            cycles->cyclesVertexId.append(pathId);
            pathId.clear();
        }
    }


    return cycles;
}

graphConnectivity Algorithms::checkConnectivity(vertexList verticesList, graphTypes graphType)
{
    if(graphType == graphTypes::orientGraph) {
        if(checkStronglyConnectivity(verticesList, graphType)) return graphConnectivity::STRONGLY;
        if(checkWeaklyConnectivity(verticesList)) return graphConnectivity::WEAKLY;
        if(checkSidedConnectivity(verticesList, graphType)) return graphConnectivity::SIDED;
    }

    return graphConnectivity::notConnectivity;
}

QList<int> Algorithms::topologicalSorting(vertexList verticesList, graphTypes graphType)
{
    if(!checkCycles(verticesList)->allCycles.isEmpty()) {
        return QList<int>();
    }

    if(verticesList.isEmpty() || graphType != graphTypes::orientGraph) {
        return QList<int>();
    }

    graphTopologicalSortWorker worker;
    for(Vertex * vertex : verticesList) {
        depthSearch(vertex, &worker);
    }

    QList<int> sorted;
    for(int k : worker.leaveList()) {
        sorted.insert(0, k);
    }

    return sorted;
}

bool Algorithms::isHaveNegativeWeight(vertexList verticesList)
{
    for(const Vertex * vertex : verticesList) {
        for(Edge * edge : vertex->outEdges()) {
            if(edge->weight() < 0) {
                return true;
            }
        }
    }

    return false;
}

bool Algorithms::isHaveNegativeCyclesFrom(int startVertexId, vertexList verticesList)
{
    Vertex *startVertex = nullptr;
    for(Vertex *v : verticesList) {
        if(v->id() == startVertexId) {
            startVertex = v;
        }
        if(startVertex != nullptr) {
            break;
        }
    }
    if(startVertex == nullptr) {
        qFatal("Vertices not found");
    }

    QVector<Edge*> edges;

    for(Vertex *v : verticesList) {
        for(Edge *ed : v->outEdges()) {
            edges.append(ed);
        }
    }

    QMap<int, int> weight;
    QMap<int, int> path;

    for(Vertex *v : verticesList) {
        weight[v->id()] = INF;
    }
    weight[startVertexId] = 0;

    for(int i = 0; i < verticesList.size() - 1; i++) {
         for(Edge *e : edges) {
             if(weight[e->targetVertex()->id()] > weight[e->sourceVertex()->id()]  + e->weight()) {
                 weight[e->targetVertex()->id()] = weight[e->sourceVertex()->id()] + e->weight();
                 path[e->targetVertex()->id()] = e->sourceVertex()->id();
             }
         }
    }

    for(Edge *e : edges) {
        if(weight[e->targetVertex()->id()] > weight[e->sourceVertex()->id()]  + e->weight()) {
            return true;
        }
    }

    return false;
}

pathToAll Algorithms::BellmanFordAlgor(int startVertexId, vertexList verticesList)
{
    Vertex *startVertex = nullptr;
    for(Vertex *v : verticesList) {
        if(v->id() == startVertexId) {
            startVertex = v;
        }
        if(startVertex != nullptr) {
            break;
        }
    }
    if(startVertex == nullptr) {
        qFatal("Vertices not found");
    }

    QVector<Edge*> edges;

    for(Vertex *v : verticesList) {
        for(Edge *ed : v->outEdges()) {
            edges.append(ed);
        }
    }

    pathToAll result;
    result.isHaveNegativeCycles = false;
    result.isHaveNegativeEdgeWeight = false;
    QMap<int, int> path;

    for(Vertex *v : verticesList) {
        result.paths[v->id()].weight = INF;
    }
    result.paths[startVertexId].weight = 0;

    for(int i = 0; i < verticesList.size() - 1; i++) {
         for(Edge *e : edges) {
             if(result.paths[e->targetVertex()->id()].weight > result.paths[e->sourceVertex()->id()].weight + e->weight()) {
                 result.paths[e->targetVertex()->id()].weight = result.paths[e->sourceVertex()->id()].weight + e->weight();
                 path[e->targetVertex()->id()] = e->sourceVertex()->id();
             }
         }
    }

    for(Edge *e : edges) {
        if(result.paths[e->targetVertex()->id()].weight > result.paths[e->sourceVertex()->id()].weight + e->weight()) {
            result.isHaveNegativeCycles = true;
            return result;
        }
    }

    for (int key : result.paths.keys()) {
        BellmanFordPathRecovery(&result.paths[key], key, path);
    }

    result.keys = result.paths.keys().toVector();

    for(int id : result.keys) {
        if(result.paths[id].weight == INF) {
            result.paths.remove(id);
        }
    }

    return result;
}

void Algorithms::BellmanFordPathRecovery(weightedPath *resultPath, int endId, const QMap<int, int> &path)
{
    if(resultPath->weight == INF) {
        return;
    }

    while(path.contains(endId)) {
        resultPath->path.insert(0, endId);
        if(endId == path[endId]) {
            break;
        }
        endId = path[endId];
    }   
    resultPath->path.insert(0, endId);
}

pathToAll Algorithms::getMinPathsDijkstra(int startVertexId, vertexList vList)
{
    Vertex *start = nullptr;
    for(Vertex *vertex : vList) {
        if(vertex->id() == startVertexId) {
            start = vertex;
            break;
        }
    }

    if(start == nullptr) {
        qFatal("Vertex not found\n");
//        return nullptr;
    }


    pathToAll *resultPath = new pathToAll;
    resultPath->isHaveNegativeCycles = false;
    resultPath->isHaveNegativeEdgeWeight = false;
    resultPath->fromVertexId = start->id();
    if(isHaveNegativeWeight(vList)) {
        resultPath->isHaveNegativeEdgeWeight = true;
        return *resultPath;
    }
    QMap<int, weightedPath> &rPath = resultPath->paths;    // resultPath

    for(Vertex *vertex : vList) {
        rPath.insert(vertex->id(), {true, INF, QVector<int>( {vertex->id()} )} );
    }
    rPath[startVertexId].weight = 0;

    Vertex *min;
    while(!vList.empty()) {
        // Пошук невідвіданої вершини з мінімальним шляхом
        min = vList[0];

        for(Vertex * vertex : vList) {
            if(rPath[min->id()].weight > rPath[vertex->id()].weight) {
                min = vertex;
            }
        }

        if(rPath[min->id()].weight == INF) {
            break;
        }

        for(Edge * e : min->outEdges()) {
            if(rPath[e->targetVertex()->id()].weight > (rPath[min->id()].weight + e->weight())) {
                rPath[e->targetVertex()->id()].weight = rPath[min->id()].weight + e->weight();
                rPath[e->targetVertex()->id()].path   = rPath[min->id()].path;
                rPath[e->targetVertex()->id()].path.append(e->targetVertex()->id());
            }
        }
        vList.removeOne(min);
    }

    resultPath->keys = rPath.keys().toVector();

    for(int id : resultPath->keys) {
        if(rPath[id].weight == INF) {
            rPath.remove(id);
        }
    }

    // need delete resultPath?
    return *resultPath;
}

weightedPath Algorithms::getMinPathsDijkstra(int startVertexId, int endVertexId, vertexList vList)
{
    pathToAll paths = getMinPathsDijkstra(startVertexId, vList);
    return paths.paths[endVertexId];
}

bool Algorithms::checkStronglyConnectivity(vertexList verticesList, graphTypes graphType)
{
    if(graphType != graphTypes::orientGraph) {
        return false;
    }
    graphMatrix *reachabilityMatrix = getReachabilityMatrix(verticesList);

    for(int row : reachabilityMatrix->rowKeys) {
        for(int col : reachabilityMatrix->colKeys) {
            if(reachabilityMatrix->matrix[row][col] != 1) {
                return false;
            }
        }
    }

    delete reachabilityMatrix;
    return true;
}

bool Algorithms::checkSidedConnectivity(vertexList verticesList, graphTypes graphType)
{
    if(graphType != graphTypes::orientGraph) {
        return false;
    }
    graphMatrix *reachabilityMatrix = getReachabilityMatrix(verticesList);

    for(int row : reachabilityMatrix->rowKeys) {
        for(int col : reachabilityMatrix->colKeys) {
            if( reachabilityMatrix->matrix[row][col] + reachabilityMatrix->matrix[col][row] == 0) {
                return false;
            }
        }
    }

    delete reachabilityMatrix;
    return true;
}

bool Algorithms::checkWeaklyConnectivity(vertexList verticesList)
{
    graphMatrix *resultMatrix = new graphMatrix;

    simpleMatrix &resMatrix = resultMatrix->matrix;

    for(const Vertex *i : verticesList) {
        resultMatrix->rowKeys.append(i->id());
        resultMatrix->colKeys.append(i->id()); // its square matrix
        for (Edge *edge : i->outEdges()) {
            resMatrix[i->id()][edge->targetVertex()->id()] = 1;
        }
        for(Edge *edge : i->inEdges()) {
            resMatrix[i->id()][edge->reverseEdge()->targetVertex()->id()] = 1;
        }
    }

    graphMatrix* newMatrix = resultMatrix;
//    int size = newMatrix->rowKeys.size();
//    if(size != newMatrix->colKeys.size()) {
//        ;
//    }

    simpleMatrix &matrix = newMatrix->matrix;

    for(int k : newMatrix->rowKeys) {
        for(int i : newMatrix->rowKeys) {
            for(int j : newMatrix->colKeys) {
                matrix[i][j] = int(matrix[i][j]) | (matrix[i][k] & matrix[k][j]);
            }
        }
    }

    for(int i : newMatrix->rowKeys) {
        matrix[i][i] = 1;
    }

    for(int row : newMatrix->rowKeys) {
        for(int col : newMatrix->colKeys) {
            if((newMatrix->matrix[row][col]) == 0) {
                return false;
            }
        }
    }
    return true;
}



bool Algorithms::isPlanarity(vertexList verticesList)
{
    // TODO:
    if (verticesList.size() <= 4) {
        return true;
    }

    return false;
}

QMap<int, int> Algorithms::coloring(vertexList verticesList)
{
    QMap<int, int> resultColors;     // id - color
    graphMatrix *adjacency = getAdjacencyMatrix(verticesList);

    for(int i : adjacency->rowKeys) {
        int nextFreeColor = 0;

        bool isContinue = true;
        while(isContinue) {
            isContinue = false;

            for (int j : adjacency->colKeys) {
                if(i == j) continue;

                bool isConnected  = (adjacency->matrix[i][j] == 1 || adjacency->matrix[j][i] == 1);
                bool isEqualColor = (resultColors.value(j, -1) == nextFreeColor);
                if (isConnected && isEqualColor) {
                    nextFreeColor++;
                    isContinue = true;
                    break;
                }
            }
        }

        resultColors[i] = nextFreeColor;
    }

    delete adjacency;
    return resultColors;
}

/**
 * @brief Пошук одного з багатьох можливих шляхів для максимального потоку і пропуск через нього максимального потоку
 * @param flowWeightMatrix  Матриця максимальних потоків
 * @param visibled Матриця відвіданих вершин
 * @param current Поточний елемент
 * @param end Кінцевий елемент, стік
 * @param maxPossibleFlow максимальний можливий потік через поточну вершину
 * @return Величина знайденоко максимального частинного потоку
 */
int Algorithms::findPath(graphMatrix &flowWeightMatrix, int current, int end, graphMatrix &currentFlowWeight, QSet<int> *visibled, int maxPossibleFlow)
{
    tempPath.path.append(current);

    bool isNeedDeleteVisibled = false;
    if(visibled == nullptr) {
        visibled = new QSet<int>;
    }
    if(current == end) {            // Якщо поточний елемент це кінцевий елемент
        if(isShowPaths) {
            qDebug() << "Weight: " << maxPossibleFlow << " path: " << tempPath.path;
        }
        tempPath.path.clear();
        return maxPossibleFlow;     // то шлях до нього знайдено
    }

    visibled->insert(current);

    for(int v : flowWeightMatrix.rowKeys) {     // Переглядаємо всі невідвідані вершини з потоком більше нуля
        int dWeight = flowWeightMatrix.matrix[current][v] + currentFlowWeight.matrix[current][v];
        if (!visibled->contains(v) && dWeight > 0) {
            int maxPossibleFl = min(maxPossibleFlow, dWeight);
            int df = findPath(flowWeightMatrix, v, end, currentFlowWeight, visibled, maxPossibleFl); // Знаходимо максимальний потік для вершин
            if (df > 0) {           // Корегуємо поточні максимальні потоки
                currentFlowWeight.matrix[current][v] -= df;
                currentFlowWeight.matrix[v][current] += df;
                return df;
            }
        }
    }

    if(isNeedDeleteVisibled) {
        delete visibled;
    }
    tempPath.path.removeLast();
    return 0;
}

void Algorithms::checkContainsVertex(int beginVertexId, int endVertexId, vertexList verticesList)
{
    Vertex *startVertex = nullptr;
    Vertex *endVertex   = nullptr;
    for(Vertex *v : verticesList) {
        if(v->id() == beginVertexId) {
            startVertex = v;
        } /*else*/  // (startVertex == endVertex) => error
        if(v->id() == endVertexId) {
            endVertex = v;
        }
        if(startVertex != nullptr && endVertex != nullptr) {
            break;
        }
    }

    if(startVertex == nullptr || endVertex == nullptr) {
        qFatal("Vertices not found");
    }
}

/**
 * @brief Пошук максимального потоку між парою вершин
 * @param beginVertexId Початкова вершина, джерело
 * @param endVertexId   Кінцева вершина, стік
 * @param vertexList    Список вершин
 * @return Величина максимального потоку
 */
flow Algorithms::FordFulkerson(int beginVertexId, int endVertexId, vertexList verticesList)
{
    flow resultFlow;        // Результуючий потік
    resultFlow.beginVertexId = beginVertexId;   // Ініціалізуємо поля структури
    resultFlow.endVertexId   = endVertexId;
    resultFlow.value         = 0;

    if(beginVertexId == endVertexId) {
        resultFlow.value = INF;
        return resultFlow;
    }

    checkContainsVertex(beginVertexId, endVertexId, verticesList);    // Перевірка наявності вершин в списку

    graphMatrix *flowWeightMatrix = getAdjacencyMatrix(verticesList, true);     // Отримуємо матрицю ваг графу

//    if(flowWeightMatrix->type != matrixType::flowWeights) {
//        qFatal("Bad matrix type");
//    }

    int addedFlow = 0;
    graphMatrix flowReultMatrix;
    do {
        addedFlow = findPath(*flowWeightMatrix, beginVertexId, endVertexId, flowReultMatrix); // Обчислення частинного потоку
        resultFlow.value += addedFlow;  // Додання частинного потоку до результуючого потоку+
    } while(addedFlow != 0);

    resultFlow.resultFlowMatrix = flowReultMatrix;
    return resultFlow;
}

graphMatrix Algorithms::_encodeMatrixToSandardIndex(const graphMatrix &matrix)
{
    graphMatrix resultMatrix;
    resultMatrix.rowKeys.reserve(matrix.rowKeys.size());
    resultMatrix.colKeys.reserve(matrix.colKeys.size());

    int i = 0;
    for(int iFrom : matrix.rowKeys) {
        resultMatrix.rowKeys.append(i);
        resultMatrix.colKeys.append(i);
        int j = 0;
        for(int jFrom : matrix.colKeys) {
            resultMatrix.matrix[i][j] = matrix.matrix[iFrom][jFrom];
            j++;
        }
        i++;
    }

    return resultMatrix;
}

graphMatrix Algorithms::_decodeMatrix(const graphMatrix &fromMatrix, QVector<int> rowKeys, QVector<int> colKeys)
{
    graphMatrix resultMatrix;
    resultMatrix.rowKeys = rowKeys;
    resultMatrix.colKeys = colKeys;

    int iFrom = 0;
    for(int i : rowKeys) {
        int jFrom = 0;
        for(int j : colKeys) {
//            qDebug() << i << j << iFrom << jFrom;
            resultMatrix.matrix[i][j] = fromMatrix.matrix[iFrom][jFrom];
            jFrom++;
        }
        iFrom++;
    }

    return resultMatrix;
}
