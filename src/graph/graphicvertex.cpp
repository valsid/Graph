#include "graphicvertex.h"

#include <QPen>
#include <QRectF>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>

#include "vertex.h"
#include "graph.h"
#include "../globalsettings.h"

#include "util/rand.h"

const qreal GraphicVertex::_circleRadius = 30;
graphItemSelectSignalEmitter *GraphicVertex::_selectSignalEmitter = new graphItemSelectSignalEmitter();

GraphicVertex::GraphicVertex(Vertex *parentVertex, QGraphicsItem *parent) :
    QAbstractGraphicsShapeItem(parent),
    _parentVertex(parentVertex),
    _isFixedPos(false)
{
    setPos(Rand::normalNumber(0, 200), Rand::normalNumber(0, 200));

    setBrush(Qt::white);
    setPen(GlobalSettings::vertexDefaultPen());
    setZValue(GlobalSettings::vertexDefaultZValue());

    setFlag(QAbstractGraphicsShapeItem::ItemIsSelectable);
    setFlag(QAbstractGraphicsShapeItem::ItemIsMovable);
    setCacheMode(DeviceCoordinateCache);
}

GraphicVertex::~GraphicVertex()
{
    removeFromScene();
}

bool GraphicVertex::isFixedPos() const
{
    return _isFixedPos;
}

void GraphicVertex::setFixedPos(bool value)
{
    _isFixedPos = value;
}

void GraphicVertex::restoreDefaultColor()
{
    setPen(GlobalSettings::vertexDefaultPen());
}

graphItemSelectSignalEmitter *GraphicVertex::selectSignalEmitterInstance()
{
    return _selectSignalEmitter;
}

void GraphicVertex::setEdgesSelect(bool value)
{
    for(Edge *e : parentVertex()->outEdges()) {
        e->graphicEdge()->setEdgeSelected(value);
    }

    for(Edge *e : parentVertex()->inEdges()) {
        e->graphicEdge()->setEdgeSelected(value);
    }
}

void GraphicVertex::updateEdgePos()
{
    foreach (Edge *edge, parentVertex()->outEdges()) {
        edge->graphicEdge()->updatePos();
    }

    foreach (Edge *edge, parentVertex()->inEdges()) {
        edge->graphicEdge()->updatePos();
    }
}

QRectF GraphicVertex::boundingRect() const
{
    qreal penWidth = pen().width();
    return QRectF( -_circleRadius - penWidth / 2,
                   -_circleRadius - penWidth / 2,
                    _circleRadius * 2 + penWidth,
                    _circleRadius * 2 + penWidth);
}

void GraphicVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option)

    if (option->state & QStyle::State_Selected)  {
        painter->setPen(GlobalSettings::vertexSelectPen());
    } else {
        painter->setPen(pen());
    }
    painter->setBrush(brush());

    painter->drawEllipse(-_circleRadius, -_circleRadius, _circleRadius * 2, _circleRadius * 2);

    if(GlobalSettings::isShowVertexId()) {
        // TODO: remove magic position
        painter->drawText(-4, 4, QString::number(parentVertex()->id()));
    }
}

QPainterPath GraphicVertex::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

QVariant GraphicVertex::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QAbstractGraphicsShapeItem::ItemSelectedChange) {
        setNodeSelect(value.toBool());
    }

    return QAbstractGraphicsShapeItem::itemChange(change, value);
}

void GraphicVertex::setNodeSelect(bool isSelect)
{
    if(isSelect) {
        setZValue(GlobalSettings::vertexSelectZValue());

        vertexSelected();
    } else {
        setZValue(GlobalSettings::vertexDefaultZValue());
    }

    setEdgesSelect(isSelect);
}

void GraphicVertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setFixedPos(true);

    vertexMoved();

    QAbstractGraphicsShapeItem::mouseMoveEvent(event);
}

void GraphicVertex::vertexSelected()
{
    QVector<int> outVertex;
    outVertex.reserve(_parentVertex->outEdges().size());
    for(Edge *v : _parentVertex->outEdges()) {
        outVertex.append(v->targetVertex()->id());
    }

    QVector<int> inVertex;
    inVertex.reserve(_parentVertex->inEdges().size());
    for(Edge *v : _parentVertex->inEdges()) {
        inVertex.append(v->targetVertex()->id());
    }

    _selectSignalEmitter->vertexSelect(this->parentVertex());
}

void GraphicVertex::vertexMoved()
{
    _selectSignalEmitter->makeEmitVertexMoved(this->parentVertex());
}

bool GraphicVertex::isNeedUpdatePos() const
{
    return isSelected();
}

void GraphicVertex::updateLinkedEdgePos()
{
    updateEdgePos();
}

void GraphicVertex::updatePosUsingForce()
{
    if(!GlobalSettings::isUsingForce() || _isFixedPos == true || scene() == nullptr || scene()->selectedItems().contains(this)) {
        return;
    }

    qreal xSpeed = 0;
    qreal ySpeed = 0;
    foreach (Vertex *item, parentVertex()->parentGraph().allVertices()) {
        QPointF vec = mapToItem(item->graphicsVertex(), 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 1.0/GlobalSettings::force1() * (dx * dx + dy * dy);
        if (l > 0) {
            xSpeed += dx / l;
            ySpeed += dy / l;
        }
    }

    double weight = (_parentVertex->outEdges().size() + _parentVertex->inEdges().size() + 1) * GlobalSettings::force2();
    QPointF vec(0, 0);

    auto updateVecFunc = [&](GraphicVertex *vertex) {
        vec = mapToItem(vertex, 0, 0);
        xSpeed -= vec.x() / weight;
        ySpeed -= vec.y() / weight;
    };

    foreach (Edge *edge, _parentVertex->outEdges()) {
        updateVecFunc(edge->targetVertex()->graphicsVertex());
    }

    foreach (Edge *edge, _parentVertex->inEdges()) {
        updateVecFunc(edge->sourceVertex()->graphicsVertex());
    }

    if (qAbs(xSpeed) < 2 && qAbs(ySpeed) < 2) {
        xSpeed = ySpeed = 0;
    }

    QRectF sceneRect = scene()->sceneRect();
    QPointF newPos = pos() + QPointF(xSpeed, ySpeed);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 20), sceneRect.right()  - 20));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top()  + 20), sceneRect.bottom() - 20));

    setPos(newPos);

    updateEdgePos();
}

qreal GraphicVertex::radius()
{
    return _circleRadius;
}

Vertex *GraphicVertex::parentVertex() const
{
    return _parentVertex;
}

void GraphicVertex::removeFromScene()
{
    if(scene() != nullptr) {
        scene()->removeItem(this);
    }

//    for(Edge *e : parentVertex()->edges()) {
//        e->grapgicEdge()->removeFromScene();
//    }

//    for(Edge *e : parentVertex()->inEdges()) {
//        e->grapgicEdge()->removeFromScene();
//    }
}
