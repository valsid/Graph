#include "graphscene.h"

#include <QMessageBox>
#include <QGraphicsItem>
#include <QList>
#include <QTimer>
#include "view/idselectdialog.h"
#include "globalsettings.h"

extern int    rand_int(int min, int max);
extern double normal_rand_number(double mean, double stddev);

#include <QMap>

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent),
    _graph(new Graph(graphTypes::orientGraph, this))
{
    _positionUpdateTimerId = startTimer(25);
    _forcePosUpdateTimerId = startTimer(50);

    connect(GlobalSettings::instance(), &GlobalSettings::penSettingsChanged,
            this,                       &GraphScene::updateAllGraphItems);


/*
    const size_t n = 10;
    GraphNode *node[n];
    for(size_t i = 0; i < n; i++) {
        node[i] = new GraphNode;
        _nodeList->append(node[i]);
    }

    for(size_t i = 0; i < (n / 2); i++) {
        node[0]->addVertices(node[i + 1]);
        node[i]->addVertices(node[i + n / 2]);
//        node[i + n / 2]->addVertices(node[i]);
    }
// */
//    int rand;

/*
    for(int i = 0; i < 10; i++) {
        rand = rand_int(1, 8);

        for(int i = 0; i < rand; i++) {
            tmp->addVertices(new GraphNode);
        }
        tmp = tmp->edges()->at( rand_int(0, rand - 1) )->vertex();
    }
// */
/*
    GraphNode *tmpNode = new GraphNode;
    _nodeList->append(tmpNode);
    GraphNode *tmpNode2;
    int rand2;
    for(int i = 0; i < 10; i++) {
        rand = normal_rand_number(4, 1) + 0.5;

        for(int i = 0; i < rand; i++) {
            tmpNode2 = new GraphNode;
            _nodeList->append(tmpNode2);
            tmpNode->addEdge(tmpNode2);
        }

        do {
            rand2 = normal_rand_number(rand / 2, 0.3) + 0.5;
        } while(!(rand2 >= 0 && rand2 < rand));
//        rand2 = rand_int(0, rand - 1);

        tmpNode = tmpNode->edges()->at( rand2 )->vertex();
    }

// / *
    for(GraphNode *r: *_nodeList) {
        r->addVertices(_nodeList->at( rand_int(0, _nodeList->size())));
    }
*/
}

GraphScene::~GraphScene()
{
//    removeAllGraphItem();
    delete _graph;
}
/*
void GraphScene::recursiveDrawNode(Vertex *node)
{

    if(node == nullptr) {
        return;
    }
    if(node->painted() == true) {
        return;
    }

    auto vertices = node->edges();
    size_t verticesNumber = vertices->size();

    addItem(node);
    node->setPainted(true);

    qreal vD = 70 + node->radius() * 2;

    qreal topPosY = node->y() - vD * (verticesNumber - 1) / 2;
    qreal posX = node->x() + vD;
    qreal posY = topPosY + rand_int(-40, 40);

    for(Edge *i : *vertices ) {
        if(i->painted() == true / *|| i->node()->painted() == true *//*) {
            continue;
        }

        if(i->vertex()->painted() == false && i->vertex()->fixedPos() == false) {
            i->vertex()->setX(posX);
            i->vertex()->setY(posY);
        }

        i->updatePos();
        addItem(i);
        i->setPainted(true);

        posY += vD;
        if(i->vertex()->painted() != true) {
            recursiveDrawNode(i->vertex());
        }
    }
}*/

void GraphScene::newNode(const newNodeData &data)
{
    Vertex *tmp = _graph->createNewVertex(&(data.vertices), data.pos, data.isFixedPos);
    addItem(tmp->graphicsVertex());

    for(Edge *edge: tmp->outEdges()) {
//        if(!items().contains(edge->graphicEdge())) {
        if(edge->graphicEdge()->scene() == nullptr) {
            addItem(edge->graphicEdge());
        }
        edge->graphicEdge()->updatePos();
    }

    for(Edge *edge: tmp->inEdges()) {
//        if(!items().contains(edge->graphicEdge())) {
        if(edge->graphicEdge()->scene() == nullptr) {
            addItem(edge->graphicEdge());
        }
        edge->graphicEdge()->updatePos();
    }
}

void GraphScene::removeSelectedVertex()
{
    QList<QGraphicsItem *> selectedItems = this->selectedItems();
    for(QGraphicsItem *item : selectedItems) {
        GraphicVertex *v = qgraphicsitem_cast<GraphicVertex *>(item);
        if(v != nullptr) {
            _graph->removeVertex(v->parentVertex()->id());
        }
    }
}

void GraphScene::addIsolatedVertex(QPointF pos)
{
    addItem(_graph->createNewVertex(nullptr, pos, true)->graphicsVertex());
}

void GraphScene::clearScene()
{
    graph()->clearGraph();
}

void GraphScene::updateAllGraphItems()
{
    for(Vertex *v : graph()->allVertices()) {
        v->graphicsVertex()->setPen(GlobalSettings::vertexDefaultPen());
        // TODO: add selected vertex pen
    }

    for(Edge *e : graph()->allEdges()) {
        e->graphicEdge()->setPen(GlobalSettings::edgeDefaultPen());
    }
}

Graph *GraphScene::graph() const
{
    return _graph;
}

void GraphScene::addNodesFromString(QString str, bool isWeight)
{
    QMessageBox warningMessage;
    warningMessage.setWindowTitle(tr("Попередження"));
    warningMessage.setIcon(QMessageBox::Warning);

    if(str.isEmpty()) {
        warningMessage.setText(tr("Файл порожній"));
        warningMessage.exec();
        return;
    }

    QTextStream stream(&str);
    stream.seek(1);

    /*unsigned*/ int verticesNumber, edgeNumber;
    stream >> verticesNumber >> edgeNumber;
    if(verticesNumber <= 0 || edgeNumber <= 0) {
        warningMessage.setText(tr("Кількість вершин чи ребер вказано невірно"));
        warningMessage.exec();
        return;
    }

    while(graph()->allVertices().size() < verticesNumber) {
        auto tmp = graph()->createNewVertex();
        addItem(tmp->graphicsVertex());
    }

    int firstVertices, secondVertices;
    int weight = 0;
    for(int i = 1; i <= edgeNumber; i++) {   //    while (!stream.atEnd()) {
        stream >> firstVertices >> secondVertices;
        if(isWeight) {
            stream >> weight;
        }
        if(firstVertices  < 1 || firstVertices  > verticesNumber ||
           secondVertices < 1 || secondVertices > verticesNumber) {
            warningMessage.setText(tr("Помилка розбору файлу\nПриблизно в рядку № ") + QString::number(i + 1));
            warningMessage.exec();
            return;
        }

        auto first  = _graph->getVertex(firstVertices);
        auto second = _graph->getVertex(secondVertices);
        if(first == nullptr || second == nullptr) {
            warningMessage.setText(tr("Помилка розбору файлу\nПриблизно в рядку № ") + QString::number(i + 1));
            warningMessage.exec();
            return;
        }

        first->addVertexLink(second, weight);
        for(Edge *edges : first->outEdges()) {
            addItem(edges->graphicEdge());
            edges->graphicEdge()->updatePos();
        }
    }
}

#include <QDialog>
#include <QLayout>
#include <QSpinBox>
#include <QPushButton>

int GraphScene::selectVertex()
{
    idSelectDialog dialog(*graph());
    dialog.exec();

    return dialog.lastSelectedVertexId();
/*
    QDialog dialog;
    dialog.setLayout(new QHBoxLayout);
    QSpinBox number;
    QPushButton okButton("Ok");
    dialog.layout()->addWidget(&number);
    dialog.layout()->addWidget(&okButton);

    dialog.exec();
    */
//    dialog.
}

void GraphScene::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == _positionUpdateTimerId) {
        for(Vertex *v : _graph->allVertices()) {
            if(v->graphicsVertex()->isNeedUpdatePos()) {
                v->graphicsVertex()->updateLinkedEdgePos();
            }
        }
    }

    if(e->timerId() == _forcePosUpdateTimerId) {
        for(Vertex *v : _graph->allVertices()) {
            // TODO: make 'isNeedForceUpdate' flag
            v->graphicsVertex()->updatePosUsingForce();
        }
    }
}
