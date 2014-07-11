#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QObject>
#include <QPen>

#include "view/settings.h"

class GlobalSettings : public QObject
{
    Q_OBJECT
    friend class Settings;

    explicit GlobalSettings(QObject *parent = 0);
public:
    static GlobalSettings *instance();

    static QPen vertexDefaultPen();
    static QPen vertexSelectPen();
    static QPen edgeDefaultPen();
    static QPen edgeSelectPen();
    static QPen highlightVertexPen();

    static qreal vertexDefaultZValue();
    static qreal vertexSelectZValue();
    static qreal edgeDefaultZValue();
    static qreal edgeSelectZValue();

    static int force1();
    static int force2();

    static bool isShowVertexId();
    static bool isShowEdgeId();
    static bool isShowEdgeWeight();
    static bool isUsingForce();

signals:
    void penSettingsChanged();
    void textShowSettingsChanged();

private:
    void makeEmitPenSettingsChanged();
    void makeEmitTextShowSettingsChanged();

    static void setVertexDefaultPen     (QPen vertexDefaultPen);
    static void setVertexSelectPen      (QPen vertexSelectPen);
    static void setEdgeDefaultPen       (QPen edgeDefaultPen);
    static void setEdgeSelectPen        (QPen edgeSelectPen);
    static void setHighlightVertexPen   (QPen highlightVertexPen);

//    static void setVertexDefaultZValue  (qreal vertexDefaultZValue);
//    static void setVertexSelectZValue   (qreal vertexSelectZValue);
//    static void setEdgeDefaultZValue    (qreal edgeDefaultZValue);
//    static void setEdgeSelectZValue     (qreal edgeSelectZValue);

    static void setShowVertexId     (bool isShowVertexId);
    static void setShowEdgeId       (bool isShowEdgeId);
    static void setShowEdgeWeight   (bool isShowEdgeWeight);
    static void setUsingForce       (bool value);

    static void setForce1(int v);
    static void setForce2(int v);

    static void setAllDefault();

    static QPen _vertexDefaultPen;
    static QPen _vertexSelectPen;
    static QPen _edgeDefaultPen;
    static QPen _edgeSelectPen;

    static QPen _highlightVertexPen;

    static qreal _vertexDefaultZValue;
    static qreal _vertexSelectZValue;
    static qreal _edgeDefaultZValue;
    static qreal _edgeSelectZValue;

    static int _tmpFroce1;
    static int _tmpForce2;

    static bool _isShowVertexId;
    static bool _isShowEdgeId;
    static bool _isShowEdgeWeight;
    static bool _isUsingForce;      // May the Force be with you

    static GlobalSettings &_instanceForEmit;   // for emit
};

#endif // GLOBALSETTINGS_H
