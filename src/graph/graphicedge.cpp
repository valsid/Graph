#include "graphicedge.h"

#include "globalsettings.h"
#include "graph.h"

#include <QGraphicsScene>
#include <QStyleOption>
#include <QtMath>

GraphicEdge::GraphicEdge(Edge &parentEdge, QGraphicsItem *parent):
    QAbstractGraphicsShapeItem(parent),
    _parentEdge(parentEdge),
    _textData(this)
{
    setPen(GlobalSettings::edgeDefaultPen());
    setZValue(GlobalSettings::edgeDefaultZValue());

//    setBrush( QBrush(Qt::black));
    setFlag(QAbstractGraphicsShapeItem::ItemIsSelectable);
    setCacheMode(DeviceCoordinateCache);

    updateTextData();
}

void GraphicEdge::removeFromScene()
{
    if(scene() != nullptr) {
        scene()->removeItem(this);
    }
    if(reverseGraphicEdge()->scene() != nullptr) {
        reverseGraphicEdge()->scene()->removeItem(reverseGraphicEdge());
    }
}

QRectF GraphicEdge::boundingRect() const
{
//    qreal penWidth = pen().width();

//    qreal arrowMaxWidth = _arrowSizeY;
    const qreal maxIdTextWidth = 10;      // magic number
//    qreal adjust = penWidth;
    qreal adjust = maxIdTextWidth;

    if(parentEdge().isLoop()) {
        return QRectF(_sourcePos.x(), _sourcePos.y() - 25, 75, 50)    // TODO: магічні числа, розмір петлі
                .normalized()
                .adjusted(-adjust, -adjust, adjust, adjust);
    }

    return QRectF(_sourcePos, QSizeF(_targetPos.x() - _sourcePos.x(),
                                      _targetPos.y() - _sourcePos.y()))
        .normalized()
        .adjusted(-adjust, -adjust, adjust, adjust);
}

void GraphicEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)


//    if (option->state & QStyle::State_Selected)  {
//        painter->setPen(GlobalSettings::edgeSelectPen());
//    } else {
        painter->setPen(pen());
//    }

    painter->setBrush(brush());

    if(currentArrowMultipler != 1) {
        QPen tmpPen = painter->pen();
        tmpPen.setWidthF(tmpPen.width() * currentArrowMultipler);
        painter->setPen(tmpPen);
    }


    if(!parentEdge().isLoop()) {
        painter->drawPath(arrowsPath(currentArrowAngle, currentArrowMultipler));
    }

//    if(eType == edgeType::additionalEdge && reverseGraphicEdge()->scene() != nullptr) {
//        return;
//    }

    painter->drawPath(linePath());

    // debug:
//    painter->drawRect( boundingRect());
}

QPainterPath GraphicEdge::shape() const
{
    return linePath() + (parentEdge().isLoop() ? QPainterPath() : arrowsPath(currentArrowAngle));
}


QPainterPath GraphicEdge::arrowsPath(qreal angle, qreal multiplier) const
{
    QPainterPath path;
    if(multiplier == 0) {
        return path;
    }

    QMatrix matrix;
    matrix.translate(_targetPos.x(), _targetPos.y());
    matrix.scale(multiplier, multiplier);
    matrix.rotate(angle);

    path.addPolygon(matrix.map(_arrow));

    if(parentEdge().targetVertex()->parentGraph().graphType() == graphTypes::notOrientGraph) {
        matrix.reset();
        matrix.translate(_sourcePos.x(), _sourcePos.y());
        matrix.scale(multiplier, multiplier);
        matrix.rotate(angle + 180);

        path.addPolygon(matrix.map(_arrow));
    }

    return path;
}

QPainterPath GraphicEdge::linePath() const
{
    QPainterPath path;

    path.moveTo(_targetPos);

    if(parentEdge().isLoop()) {
        path.cubicTo(_targetPos.x() + 100, _targetPos.y() - 100, _targetPos.x() + 100, _targetPos.y() + 100, _sourcePos.x(), _sourcePos.y());
    } else {
        path.lineTo(_sourcePos);
    }

    return path;
}

void GraphicEdge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    parentEdge().swapDirection();

    updatePos();
}

const Edge &GraphicEdge::parentEdge() const
{
    return _parentEdge;
}

GraphicEdge *GraphicEdge::reverseGraphicEdge()
{
    return parentEdge().reverseEdge()->graphicEdge();
}

void GraphicEdge::setEdgeSelected(bool selected)
{
    if(selected) {
        setPen(GlobalSettings::edgeSelectPen());
        setZValue(GlobalSettings::edgeSelectZValue());

        reverseGraphicEdge()->setPen(GlobalSettings::edgeSelectPen());
        reverseGraphicEdge()->setZValue(GlobalSettings::edgeSelectZValue());
    } else {
        setPen(GlobalSettings::edgeDefaultPen());
        setZValue(GlobalSettings::edgeDefaultZValue());

        reverseGraphicEdge()->setPen(GlobalSettings::edgeDefaultPen());
        reverseGraphicEdge()->setZValue(GlobalSettings::edgeDefaultZValue());
    }

//    setSelected(selected);    // TODO: have bug
//    reverseGraphicEdge()->setSelected(selected);
}

QPointF GraphicEdge::firstPoint() const
{
    return _sourcePos;
}

QPointF GraphicEdge::secondPoint() const
{
    return _targetPos;
}

Edge &GraphicEdge::parentEdge()
{
    return _parentEdge;
}

void GraphicEdge::updatePos()
{
    updateTextPos();
    updateTextData();

    qreal radius = GraphicVertex::radius();

    QLineF line(parentEdge().sourceVertex()->graphicsVertex()->pos(), parentEdge().targetVertex()->graphicsVertex()->pos());

    qreal dx = line.dx();
    qreal dy = line.dy();

//    qreal dx = vertex()->graphicsVertex()->pos().x() - sourceVertex()->graphicsVertex()->pos().x();
//    qreal dy = vertex()->graphicsVertex()->pos().y() - sourceVertex()->graphicsVertex()->pos().y();

    qreal radiansAngle = ((dx != 0) ? qAtan(dy / dx) : M_PI_2);
    if(dx < 0) {
        if(dy < 0) {
            radiansAngle -= M_PI;
        } else {
            radiansAngle += M_PI;
        }
    }

    currentArrowAngle = qRadiansToDegrees(radiansAngle);


    qreal lineLengthBetweenCenters = dx / qFastCos(radiansAngle); // = line.length();

     if(lineLengthBetweenCenters > radius * 2) {
         qreal LineLengthBetweenBorders = lineLengthBetweenCenters - 2 * radius;
         if(LineLengthBetweenBorders <=_arrowSizeX * 3 && !parentEdge().isLoop()) {
             currentArrowMultipler = LineLengthBetweenBorders / (_arrowSizeX * 3);
         } else {
             currentArrowMultipler = 1;
         }

//         _secondPoint = line.pointAt(radius / lineLength);
//         _firstPoint  = line.pointAt(1 - radius / lineLength);
//         qreal cos = dx / lineLength;
//         qreal sin = dy / lineLength;
         qreal x = radius * qFastCos(radiansAngle);
         qreal y = radius * qFastSin(radiansAngle);

         qreal sourceX  = parentEdge().sourceVertex()->graphicsVertex()->x() + x;
         qreal sourceY  = parentEdge().sourceVertex()->graphicsVertex()->y() + y;
         qreal targetX = parentEdge().targetVertex()->graphicsVertex()->x() - x;
         qreal targetY = parentEdge().targetVertex()->graphicsVertex()->y() - y;

         _sourcePos.setX(sourceX);
         _sourcePos.setY(sourceY);
         _targetPos.setX(targetX);
         _targetPos.setY(targetY);
     } else {
         _sourcePos = _targetPos = parentEdge().targetVertex()->graphicsVertex()->pos();
         currentArrowMultipler = 1;
     }

     prepareGeometryChange();
}

void GraphicEdge::updateTextData()
{
    QString textData;

    if(GlobalSettings::isShowEdgeWeight()) {
        textData.append(QString::number(parentEdge().weight()));
    }

    if(GlobalSettings::isShowEdgeId()) {
        if(GlobalSettings::isShowEdgeWeight()) {
            textData.append("\n");
        }
        textData.append(QString::number(parentEdge().id()));
    }

    _textData.setPlainText(textData);
}

void GraphicEdge::updateTextPos()
{
    QLineF line(parentEdge().sourceVertex()->graphicsVertex()->pos(), parentEdge().targetVertex()->graphicsVertex()->pos());
    QPointF textPoint = line.pointAt(0.5);
    if(parentEdge().isLoop()) {
        textPoint += QPointF(40, -_textData.boundingRect().height() / 2);
    } else {
        textPoint += QPointF(7, 7);
    }
    _textData.setPos(textPoint);
}
