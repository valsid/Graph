#include "graphitemselectsignalemitter.h"

graphItemSelectSignalEmitter::graphItemSelectSignalEmitter(QObject *parent) :
    QObject(parent)
{
}

void graphItemSelectSignalEmitter::vertexSelect(int id, QVector<int> outVertexId, QVector<int> inVertexId, QPointF pos, bool isFixedPos)
{
    vertexDataForSelection tmp;
    tmp.id = id;
    tmp.isFixed = isFixedPos;
    tmp.pos = pos;
    tmp.inVerticesID = inVertexId;
    tmp.outVerticesID = outVertexId;

    emit vertexSelected(tmp);
}

void graphItemSelectSignalEmitter::vertexSelect(Vertex *vertex)
{
    emit vertexSelected_(vertex);
}

void graphItemSelectSignalEmitter::makeEmitVertexMoved(Vertex *vertex)
{
    emit vertexMoved(vertex);
}
