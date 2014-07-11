#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);

//    static bool isShowVertexId();
//    static bool isShowEdgeId();

signals:

public slots:

private:
//    static bool showVertexID;
//    static bool showEdgeID;

//    static QColor vertexDefaultColor;
//    static QColor vertexSelectColor;
//    static QColor vertexSearchColor;

//    static QColor edgeDefaultColor;
//    static QColor edgeSelectColor;
};

#endif // SETTINGS_H
