#ifndef GRAPHICEDGE_H
#define GRAPHICEDGE_H

#include <QAbstractGraphicsShapeItem>
class Edge;

#include <QDebug>

class GraphicEdge : public QAbstractGraphicsShapeItem
{
public:
    GraphicEdge(Edge &parentEdge, QGraphicsItem *parent = nullptr);

    QPointF firstPoint()  const;
    QPointF secondPoint() const;

          Edge &parentEdge();
    const Edge &parentEdge() const;

    GraphicEdge *reverseGraphicEdge();

    void setEdgeSelected(bool selected);

    void removeFromScene();

    void updatePos();
    void updateTextData();
    void updateTextPos();   // TODO: temp

protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QPainterPath arrowsPath(qreal angle, qreal multiplier = 1) const;
    QPainterPath linePath() const;

private:
    // disable parent method
    void setPos(const QPointF &pos);
    void setX(qreal x);
    void setY(qreal y);
    qreal x();
    qreal y();

    // for paint
    QPointF _sourcePos;
    QPointF _targetPos;
    bool    _isPainted;

    qreal currentArrowAngle = 0;
    qreal currentArrowMultipler = 1;

    const qreal _arrowSizeX = 15;   // != 0
    const qreal _arrowSizeY = 5;
    const QPolygonF _arrow = QPolygonF(QVector<QPointF>( {QPoint(0, 0),          // Optimize...
                                                          QPoint(-_arrowSizeX, _arrowSizeY),
                                                          QPoint(-_arrowSizeX, -_arrowSizeY),
                                                          QPoint(0, 0)}));
    Edge &_parentEdge;
    QGraphicsTextItem _textData;
};

#endif // GRAPHICEDGE_H
