#ifndef GRAPHITEMSELECTSIGNALEMITTER_H
#define GRAPHITEMSELECTSIGNALEMITTER_H

#include <QObject>
#include "nodeData.h"

class Vertex;

class graphItemSelectSignalEmitter : public QObject
{
    Q_OBJECT
public:
    explicit graphItemSelectSignalEmitter(QObject *parent = 0);

    void vertexSelect(int id, QVector<int> outVertexId, QVector<int> inVertexId, QPointF pos, bool isFixedPos);
//    void vertexPosUpdate(QPointF, bool isFixedPos);

    void vertexSelect(Vertex *vertex);
    void makeEmitVertexMoved(Vertex *vertex);

signals:
    void vertexSelected(vertexDataForSelection data);
    void vertexSelected_(Vertex *v);
    void vertexMoved(Vertex *v);

//    void edgeSelected();

public slots:

};

#endif // GRAPHITEMSELECTSIGNALEMITTER_H
