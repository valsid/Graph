#ifndef GRAPHLINK_H
#define GRAPHLINK_H

#include <QAbstractGraphicsShapeItem>
#include <QTextItem>
#include <QSharedPointer>

#include "graphicedge.h"

class Vertex;

enum class edgeType {mainEdge, additionalEdge};

class Edge
{
    explicit Edge(Vertex *targetNode, Edge *reverseEdge,
                  QSharedPointer<GraphicEdge> graphicEdge,
                  int weight, int id, bool isLoop);

public:
    explicit Edge(Vertex *sourceNode, Vertex *targetNode, int weight = 0);
    ~Edge();

    void setWeight(int weight);
    int  weight() const;
    int  id()     const;

    Vertex *targetVertex() const;
    Vertex *sourceVertex() const;
    Edge   *reverseEdge()  const;

    void swapDirection();
    void removeLinks();

    bool isLoop() const;

    GraphicEdge *graphicEdge() const;

private:
    Q_DISABLE_COPY(Edge)

    Vertex *_vertex;
    Edge   *_reverseEdge;
    int _weight;
    int _id;

    bool _isLoop;
    bool _isLinked;
    bool _isPrepareToDelete;     // Using in destructor
    static int _idCounter;

    QSharedPointer<GraphicEdge> _graphicEdge;

    edgeType eType;
};


#endif // GRAPHLINK_H
