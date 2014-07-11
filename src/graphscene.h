#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>

#include "graph/vertex.h"
#include "graph/edge.h"

#include "graph/graph.h"

#include "structures/nodeData.h"
#include "algorithm/algorithms.h"

#include "view/graphdataview.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = 0);
    ~GraphScene();

    int selectVertex();
    Graph* graph() const;

protected:
    void timerEvent(QTimerEvent *e);

public slots:
    void addNodesFromString(QString str, bool isWeight = true);

    void newNode(const newNodeData &data);
    void removeSelectedVertex();
    void addIsolatedVertex(QPointF pos);

    void updateAllGraphItems();
    void clearScene();

private:
    int _positionUpdateTimerId;
    int _forcePosUpdateTimerId;

    Graph *_graph;
};

#endif // GRAPHSCENE_H
