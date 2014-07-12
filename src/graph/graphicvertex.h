#ifndef GRAPHICVERTEX_H
#define GRAPHICVERTEX_H

#include <QAbstractGraphicsShapeItem>
#include "../structures/graphitemselectsignalemitter.h"

class Vertex;

class GraphicVertex : public QAbstractGraphicsShapeItem
{
public:
    explicit GraphicVertex(Vertex *parentVertex, QGraphicsItem *parent = nullptr);
    virtual ~GraphicVertex();

    bool isFixedPos() const;
    void setFixedPos(bool value);

    void restoreDefaultColor();
    static graphItemSelectSignalEmitter *selectSignalEmitterInstance();

    void updateEdgePos();
    void updateLinkedEdgePos();
    void updatePosUsingForce();

    static qreal radius();
    Vertex *parentVertex() const;

    void removeFromScene();

    bool isNeedUpdatePos() const;

protected:
    void setEdgesSelect(bool value);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    void setNodeSelect(bool isSelect);

    Vertex *_parentVertex;
    bool _isFixedPos;
    static const qreal _circleRadius; // px
    static graphItemSelectSignalEmitter *_selectSignalEmitter;

    void vertexSelected();
    void vertexMoved();
};

#endif // GRAPHICVERTEX_H
