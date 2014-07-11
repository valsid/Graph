#include "globalsettings.h"

// TODO: select color

QPen GlobalSettings::_vertexDefaultPen   = QPen(QColor(0, 0, 0, 128), 2);
QPen GlobalSettings::_vertexSelectPen    = QPen(QColor(255, 0, 0, 255), 2);
QPen GlobalSettings::_edgeDefaultPen     = QPen(QColor(0, 0, 0, 128), 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
QPen GlobalSettings::_edgeSelectPen      = QPen(QColor(255, 0, 0, 255), 2, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
QPen GlobalSettings::_highlightVertexPen = GlobalSettings::edgeSelectPen();


qreal GlobalSettings::_vertexDefaultZValue = 10;
qreal GlobalSettings::_vertexSelectZValue  = 11;
qreal GlobalSettings::_edgeDefaultZValue   = 5;
qreal GlobalSettings::_edgeSelectZValue    = 6;

bool GlobalSettings::_isShowVertexId    = true;
bool GlobalSettings::_isShowEdgeId      = false;
bool GlobalSettings::_isShowEdgeWeight  = true;
bool GlobalSettings::_isUsingForce      = false;

GlobalSettings& GlobalSettings::_instanceForEmit = *(new GlobalSettings(nullptr));

int GlobalSettings::_tmpFroce1 = 50;
int GlobalSettings::_tmpForce2 = 30;


GlobalSettings::GlobalSettings(QObject *parent) :
    QObject(parent)
{
}

void GlobalSettings::setAllDefault()
{

}

bool GlobalSettings::isShowEdgeWeight()
{
    return _isShowEdgeWeight;
}

bool GlobalSettings::isUsingForce()
{
    return _isUsingForce;
}

void GlobalSettings::setUsingForce(bool value)
{
    _isUsingForce = value;
}


void GlobalSettings::makeEmitPenSettingsChanged()
{
    emit penSettingsChanged();
}

void GlobalSettings::makeEmitTextShowSettingsChanged()
{
    emit textShowSettingsChanged();
}

void GlobalSettings::setShowEdgeWeight(bool isShowEdgeWeight)
{
    _isShowEdgeWeight = isShowEdgeWeight;
    instance()->makeEmitTextShowSettingsChanged();
}

bool GlobalSettings::isShowEdgeId()
{
    return _isShowEdgeId;
}

void GlobalSettings::setShowEdgeId(bool isShowEdgeId)
{
    _isShowEdgeId = isShowEdgeId;
    instance()->makeEmitTextShowSettingsChanged();
}

bool GlobalSettings::isShowVertexId()
{
    return _isShowVertexId;
}

QPen GlobalSettings::highlightVertexPen()
{
    return _highlightVertexPen;
}

qreal GlobalSettings::edgeSelectZValue()
{
    return _edgeSelectZValue;
}

int GlobalSettings::force1()
{
    return _tmpFroce1;
}

int GlobalSettings::force2()
{
    return _tmpForce2;
}

//void GlobalSettings::setEdgeSelectZValue(qreal edgeSelectZValue)
//{
//    _edgeSelectZValue = edgeSelectZValue;
//}

void GlobalSettings::setShowVertexId(bool isShowVertexId)
{
    _isShowVertexId = isShowVertexId;
    instance()->makeEmitTextShowSettingsChanged();
}

void GlobalSettings::setForce1(int v)
{
    if(v == 0) {
        return;
    }
    _tmpFroce1 = v;
}

void GlobalSettings::setForce2(int v)
{
    if(v == 0) {
        return;
    }
    _tmpForce2 = v;
}

qreal GlobalSettings::edgeDefaultZValue()
{
    return _edgeDefaultZValue;
}

//void GlobalSettings::setEdgeDefaultZValue(qreal edgeDefaultZValue)
//{
//    _edgeDefaultZValue = edgeDefaultZValue;
//}

qreal GlobalSettings::vertexSelectZValue()
{
    return _vertexSelectZValue;
}

//void GlobalSettings::setVertexSelectZValue(qreal vertexSelectZValue)
//{
//    _vertexSelectZValue = vertexSelectZValue;
//}

qreal GlobalSettings::vertexDefaultZValue()
{
    return _vertexDefaultZValue;
}

//void GlobalSettings::setVertexDefaultZValue(qreal vertexDefaultZValue)
//{
//    _vertexDefaultZValue = vertexDefaultZValue;
//}

QPen GlobalSettings::edgeSelectPen()
{
    return _edgeSelectPen;
}

void GlobalSettings::setEdgeSelectPen(QPen edgeSelectPen)
{
    _edgeSelectPen = edgeSelectPen;
    instance()->makeEmitPenSettingsChanged();
}

void GlobalSettings::setHighlightVertexPen(QPen highlightVertexPen)
{
    _highlightVertexPen = highlightVertexPen;
    instance()->makeEmitPenSettingsChanged();
}

QPen GlobalSettings::edgeDefaultPen()
{
    return _edgeDefaultPen;
}

void GlobalSettings::setEdgeDefaultPen(QPen edgeDefaultPen)
{
    _edgeDefaultPen = edgeDefaultPen;
    instance()->makeEmitPenSettingsChanged();
}

QPen GlobalSettings::vertexSelectPen()
{
    return _vertexSelectPen;
}

void GlobalSettings::setVertexSelectPen(QPen vertexSelectPen)
{
    _vertexSelectPen = vertexSelectPen;
    instance()->makeEmitPenSettingsChanged();
}

GlobalSettings *GlobalSettings::instance()
{
    return &_instanceForEmit;
}

QPen GlobalSettings::vertexDefaultPen()
{
    return _vertexDefaultPen;
}

void GlobalSettings::setVertexDefaultPen(QPen vertexDefaultPen)
{
    _vertexDefaultPen = vertexDefaultPen;
    instance()->makeEmitPenSettingsChanged();
}
