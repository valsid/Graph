#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include <QVBoxLayout>
#include "graph/vertex.h"
#include "structures/nodeData.h"
#include "structures/directpushbutton.h"

namespace Ui {
class Controls;
}

struct edgeListRowData {
    edgeListRowData(int vertexId, int weight):
        edgeListRowData(vertexId, weight, QString::number(vertexId), QString::number(weight))
    { }

    edgeListRowData(int vertexId, int weight, QString textId, QString textWeight):
        vertexId(vertexId),
        weight(weight),
        textVertexId(textId),
        textWeight  (textWeight)
    { }

    int vertexId;
    int weight;
    DirectPushButton direction;

    QString textVertexId;
    QString textWeight;

    QWidget *widgetLink;
};

class Controls : public QWidget
{
    Q_OBJECT
public:
    explicit Controls(QWidget *parent = 0);
    ~Controls();

signals:
    void newNode(newNodeData data);
    void clearScene();

public slots:
    void vertexSelected(Vertex *v);
    void selectedVertexMoved(Vertex *v);

private slots:
    void on_fixedPosCheckBox_toggled(bool checked);

    void on_okButton_clicked();

    void on_nodeListAddButton_clicked();

    void on_nodeListDelButton_clicked();

    void on_nodeListClearButton_clicked();

    void on_nodeListSpinBox_returnPressed();

    void on_nodeListWeightSpinBox_returnPressed();

    void on_backToAddVertex_clicked();

private:
    void updateDataFromSelectedVertex();

    Ui::Controls *ui;

    void clearNodeListItems();

    void addEdgeToView(edgeListRowData *row);

    Vertex *_currentSelectVertex;

    QHash<int, edgeListRowData*> edgeList;
};

#endif // CONTROLS_H
