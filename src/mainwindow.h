#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>

#include "graphScene.h"
#include "graph/vertex.h"
#include "view/controls.h"
#include "view/settings.h"
#include <QList>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    QString *getTextGraphFromFile();

    /**
     *
     * @full
     * file format:
     *    'w' at the beginning of the file - weighted graph
     * or 'n' at the beginning of the file - not weighted graph
     * %vertex count% %edge count%
     * {%source edge id% %end edge id% %edge weight(for weighted graph%}
     *
     * example:
     * "w
     * 4 5
     * 1 2 1000
     * 1 3 1000
     * 2 3 1
     * 2 4 1000
     * 3 4 1000"
     */
    void on_action_addVericesFromFile_triggered();

    void on_action_showHangingVertex_triggered();

    void on_action_showIsolatedVertex_triggered();

    void on_action_showIncidenceMatrix_triggered();

    void on_action_showAdjacencyMatrix_triggered();

    void on_action_showDistanceMatrix_triggered();

    void on_action_showReachabilityMatrix_triggered();

    void on_action_checkCycles_triggered();

    void on_action_breadthSearch_triggered();

    void on_action_depthSearch_triggered();

    void on_action_checkConnectivity_triggered();

    void on_action_showWeightedDistanceMatrix_triggered();

    void on_action_DijkstraMinPaths_triggered();

    void on_action_BellmanFord_triggered();

    void on_action_isPlanarity_triggered();

    void on_action_coloring_triggered();

    void on_action_clearScene_triggered();

    void on_action_FordFulkerson_triggered();

    void on_action_topologicalSorting_triggered();

    void on_action_showVertexDegree_triggered();

    void on_action_saveGraph_triggered();

    void on_action_inverseGraph_triggered();

private:
    Ui::MainWindow *ui;

//    QString getTextGraphFromFile(QUrl path);

    GraphScene  *_scene;
    MatrixView  *_matrixViewWindow;
    Controls    *_controlsWidget;
    Settings    *_settingWindow;
};

#endif // MAINWINDOW_H
