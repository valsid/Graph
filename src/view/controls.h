#ifndef CONTROLS_H
#define CONTROLS_H

#include <QWidget>
#include "graph/vertex.h"
#include "structures/nodeData.h"
#include "structures/mydirectpushbutton.h"

namespace Ui {
class Controls;
}

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
    void vertexSelected(vertexDataForSelection data);

private slots:
    void on_fixedPosCheckBox_toggled(bool checked);

    void on_okButton_clicked();

    void on_nodeListAddButton_clicked();

    void on_nodeListDelButton_clicked();

    void on_nodeListClearButton_clicked();

//    void on_clearSceneButton_clicked();

    void on_nodeListSpinBox_returnPressed();

    void on_nodeListWeightSpinBox_returnPressed();


//    void edgeSelected(int id, );

    void on_backToAddVertex_clicked();

private:
    void addEdgesToList(QString id, QString weight);

    Ui::Controls *ui;
    QList<int> *_vertices;
    QList<int> *_verticesWeight;
    QList<myDirectPushButton*> _verticesDirect;

    void clearNodeListItems();
};

#endif // CONTROLS_H
