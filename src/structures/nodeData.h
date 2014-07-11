#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <QPoint>
#include <QVector>

enum class directions{INPUT, OUTPUT};

struct edgeData {
    int vertexId;
    int vertexWeight;
    directions direction;
};

struct newNodeData
{
    bool isFixedPos             = false;
    QPoint pos                  = QPoint(0, 0);
    QVector<edgeData> vertices;
};

struct vertexDataForSelection
{
    int id       = -1;
    bool isFixed = false;
    QPointF pos  = QPoint(0, 0);
    QVector<int> outVerticesID;
    QVector<int> inVerticesID;
};

#endif // GRAPHDATA_H
