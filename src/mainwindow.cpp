#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "algorithm/getallvertexlistinordervorker.h"
#include "view/settings.h"

#include <QFileDialog>
#include <QUrl>
#include <QMessageBox>

#include "util/rand.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _scene            = new GraphScene(this);
    _controlsWidget   = new Controls(this);
    _matrixViewWindow = new MatrixView(this);
    _settingWindow    = new Settings(this);

    ui->graphView->setScene(_scene);
    ui->mainToolBar->addWidget(_controlsWidget);

    connect(_controlsWidget, &Controls::newNode,
            _scene,          &GraphScene::newNode);

    connect(_controlsWidget, &Controls::clearScene,
            _scene,          &GraphScene::clearScene);

    connect(ui->action_openSettings, &QAction::triggered,
            _settingWindow,          &QWidget::show);

    connect(GraphicVertex::selectSignalEmitterInstance(), &graphItemSelectSignalEmitter::vertexSelected_,
            _controlsWidget,                              &Controls::vertexSelected);

    connect(GraphicVertex::selectSignalEmitterInstance(), &graphItemSelectSignalEmitter::vertexMoved,
            _controlsWidget,                              &Controls::selectedVertexMoved);

    connect(ui->action_forTest, &QAction::triggered,
            _scene->graph(),     &Graph::forTest);

    connect(ui->graphView,   &GraphView::removeSelectedVertex,
            _scene,          &GraphScene::removeSelectedVertex);

    connect(ui->graphView,  &GraphView::addIsolatedVertex,
            _scene,         &GraphScene::addIsolatedVertex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString *MainWindow::getTextGraphFromFile()
{
        QString str = QFileDialog::getOpenFileName(this, tr("Open graph"), tr(""), tr("txt graph data (*.txt)"));

        if(str.isEmpty()) {
            return nullptr;
        }

        QFile file(str);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return nullptr;
        }

        QTextStream fileStream(&file);

        QString *tmp = new QString(fileStream.readAll());
        file.close();

        return tmp;
}

void MainWindow::on_action_addVericesFromFile_triggered()
{
    QScopedPointer<QString> textGraph(getTextGraphFromFile());
    if(textGraph.isNull()) {
        return;
    }

    if(textGraph->at(0) == 'w' || textGraph->at(0) == 'n') {
        _scene->addNodesFromString(*textGraph, textGraph->at(0) == 'w');
    } else {
        QMessageBox::warning(this, tr("Попередження"), tr("Помилка читання типу графа"));
    }
}

void MainWindow::on_action_showHangingVertex_triggered()
{
    auto vertices = _scene->graph()->hangingVertex();
    _matrixViewWindow->showVertexList(vertices, tr("Висячі вершини"));
    delete vertices;
}

void MainWindow::on_action_showIsolatedVertex_triggered()
{
    auto vertices = _scene->graph()->isolatedVertex();
    _matrixViewWindow->showVertexList(vertices, tr("Ізольовані вершини"));
    delete vertices;
}

void MainWindow::on_action_showIncidenceMatrix_triggered()
{
    auto matrix = _scene->graph()->incidenceMatrix();
    _matrixViewWindow->showMatrix(matrix, tr("Матриця інцидентності"));
    delete matrix;
}

void MainWindow::on_action_showAdjacencyMatrix_triggered()
{
    auto matrix = _scene->graph()->adjacencyMatrix();
    _matrixViewWindow->showMatrix(matrix, tr("Матриця суміжності"));
    delete matrix;
}

void MainWindow::on_action_showDistanceMatrix_triggered()
{
    auto matrix = _scene->graph()->distanceMatrix();
    _matrixViewWindow->showMatrix(matrix, tr("Матриця відстаней"));
    delete matrix;
}

void MainWindow::on_action_showReachabilityMatrix_triggered()
{
    auto matrix = _scene->graph()->reachabilityMatrix();
    _matrixViewWindow->showMatrix(matrix, tr("Матриця досяжності"));
    delete matrix;
}

void MainWindow::on_action_checkCycles_triggered()
{
    graphCycles *cycles = _scene->graph()->checkCycles();
    _matrixViewWindow->showManyVertexList(cycles->cyclesVertexId, tr("Деякі цикли з графу"));
    delete cycles;
}

void MainWindow::on_action_breadthSearch_triggered()
{
    int vertexId = _scene->selectVertex();
    if(vertexId == -1) {
        return;
    }

    getVertexSearchOrderWorker w;
    _scene->graph()->breadthSearch(vertexId, &w);
    _scene->graph()->highlightVertex(w.allItem().toList(), 600);
}

void MainWindow::on_action_depthSearch_triggered()
{
    int vertexId = _scene->selectVertex();
    if(vertexId == -1) {
        return;
    }

    getVertexSearchOrderWorker w;
    _scene->graph()->depthSearch(vertexId, &w);
    _scene->graph()->highlightVertex(w.allItem().toList(), 600);
}

void MainWindow::on_action_checkConnectivity_triggered()
{
    auto connectType = _scene->graph()->connectivity();
    _matrixViewWindow->showGraphConnectivity(connectType, tr("Зв’язність графу"));
}

void MainWindow::on_action_showWeightedDistanceMatrix_triggered()
{
    auto matrix = _scene->graph()->weightedDistanceMatrix();
    _matrixViewWindow->showMatrix(matrix, tr("Матриця відстаней (зваж.)"));
    delete matrix;
}

void MainWindow::on_action_DijkstraMinPaths_triggered()
{
    int vertexId = _scene->selectVertex();
    if(vertexId == -1) {
        return;
    }

    auto v = _scene->graph()->minPathsDijkstra(vertexId);
    _matrixViewWindow->showPathToAll(v, tr("Шлях"));
}

void MainWindow::on_action_BellmanFord_triggered()
{
    int vertexId = _scene->selectVertex();
    if(vertexId == -1) {
        return;
    }

    auto v = _scene->graph()->minPathsBellmanFord(vertexId);
    _matrixViewWindow->showPathToAll(v, tr("Шлях"));
}

void MainWindow::on_action_isPlanarity_triggered()
{
    if(_scene->graph()->isPlanarity()) {
        _matrixViewWindow->showSimpleString("Граф планарний", "Планарність графу");
    } else {
        _matrixViewWindow->showSimpleString("Граф не планарний", "Планарність графу");
    }
}

void MainWindow::on_action_coloring_triggered()
{
    QMap<int, QColor> colorTable;
    colorTable.insert(0, Qt::red);
    colorTable.insert(1, Qt::blue);
    colorTable.insert(2, Qt::black);
    colorTable.insert(3, Qt::cyan);
    colorTable.insert(4, Qt::magenta);
    colorTable.insert(5, Qt::darkRed);
    colorTable.insert(6, Qt::darkGreen);
    colorTable.insert(7, Qt::darkBlue);
    colorTable.insert(7, Qt::darkCyan);
    colorTable.insert(8, Qt::darkMagenta);
    QMap<int, int> resultColors = _scene->graph()->coloring();

    for(int key : resultColors.keys()) {
        Vertex *v = _scene->graph()->getVertex(key);
        if(v == nullptr) {
            continue;
        }
        QPen newPen = v->graphicsVertex()->pen();
        if(!colorTable.contains(resultColors[key])) {
            colorTable.insert(resultColors[key], QColor(Rand::intNumber(0, 255), Rand::intNumber(0, 255), Rand::intNumber(0, 255)));
        }

        newPen.setColor(colorTable[resultColors[key]]);
        v->graphicsVertex()->setPen(newPen);
    }

    // TODO: restore color
}

void MainWindow::on_action_clearScene_triggered()
{
    _scene->clearScene();
}

void MainWindow::on_action_FordFulkerson_triggered()
{
    int startVertexId = _scene->selectVertex();
    if(startVertexId == -1) {
        return;
    }
    int endVertexId = _scene->selectVertex();
    if(endVertexId == -1) {
        return;
    }
    flow maxFlow = _scene->graph()->maxFlowFordFulkerson(startVertexId, endVertexId);
    if(maxFlow.value != INF) {
        _matrixViewWindow->showSimpleString("Максималний потік між вершиною "
                                      + QString::number(maxFlow.beginVertexId)
                                      + " та вершиною "
                                      + QString::number(maxFlow.endVertexId)
                                      + " дорівнює "
                                      + QString::number(maxFlow.value),
                                            "Максимальний потік");
    } else {
        _matrixViewWindow->showSimpleString("Максималний потік між вершиною "
                                      + QString::number(maxFlow.beginVertexId)
                                      + " та вершиною "
                                      + QString::number(maxFlow.endVertexId)
                                      + " дорівнює нескінченності",
                                            "Максимальний потік");
    }
}

void MainWindow::on_action_topologicalSorting_triggered()
{
    QList<int> topologicalSorting = _scene->graph()->topologicalSorting();

    QString sorting;
    if(!topologicalSorting.isEmpty()) {
        sorting.reserve(topologicalSorting.size() * 4);
        for(int v : topologicalSorting) {
            sorting.append(QString::number(v));
            sorting.append(" - ");
        }
        sorting.resize(sorting.size() - 3);
    } else {
        sorting = tr("Топологічне сортування, для даного графу, неможливе");
    }

    _matrixViewWindow->showSimpleString(sorting, tr("Топологічне сортування"));
}

void MainWindow::on_action_showVertexDegree_triggered()
{
    auto degree = _scene->graph()->vertexDegree();
    _matrixViewWindow->showVertexDegree(degree, tr("Степені6 вершин графу"));
}

void MainWindow::on_action_saveGraph_triggered()
{
    QString str = QFileDialog::getSaveFileName(this, tr("Open graph"), tr(""), tr("txt graph data (*.txt)"));

    if(str.isEmpty()) {
        return;
    }

    QFile file(str);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
        QTextStream outStream(&file);
        outStream << _scene->graph()->toTextGraph();
    }

    file.close();
}

void MainWindow::on_action_inverseGraph_triggered()
{
    _scene->graph()->inverseGraph();
}
