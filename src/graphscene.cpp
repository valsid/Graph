#include "graphscene.h"

#include <QGraphicsItem>
#include <QMessageBox>
#include <QTimer>
#include <QList>
#include <QMap>
#include "view/idselectdialog.h"
#include "globalsettings.h"

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent),
    _graph(new Graph(graphTypes::orientGraph, this))
{
    _positionUpdateTimerId = startTimer(25);
    _forcePosUpdateTimerId = startTimer(50);

    connect(GlobalSettings::instance(), &GlobalSettings::penSettingsChanged,
            this,                       &GraphScene::updateAllGraphItems);
}

GraphScene::~GraphScene()
{
    delete _graph;
}

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
    for(QGraphicsItem *item : selectedItems()) {
        GraphicVertex *v = qgraphicsitem_cast<GraphicVertex *>(item);
        if(v != nullptr) {
//            qDebug() << "remove" << v->parentVertex()->id();
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

int GraphScene::selectVertex()
{
    idSelectDialog dialog(*graph());
    dialog.exec();

    return dialog.lastSelectedVertexId();
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
