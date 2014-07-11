#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <QDialog>
#include "../algorithm/algorithms.h"
#include <QAbstractTableModel>
#include <QDebug>


namespace Ui {
class MatrixView;
}

class MatrixView : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixView(QWidget *parent = 0);
    ~MatrixView();

    void showMatrix(graphMatrix *matrix, QString title, QString addInfo = "");
    void showVertexList(QVector<int> *vertices, QString title, QString addInfo = "");
    void showVertexDegree(QVector<degree> *degreeV, QString title, QString addInfo = "");
    void showManyVertexList(const QVector<QVector<int>> &list, QString title, QString addInfo = "");
    void showGraphConnectivity(const graphConnectivity &connectivity, QString title, QString addInfo = "");
    void showPathToAll(const pathToAll &paths, QString title, QString addInfo = "");

    void showSimpleString(const QString &string, QString title, QString addInfo = "");
    void showTable(QAbstractItemModel *model, QString title, QString addInfo = "");

private slots:
    void on_OKButton_clicked();

private:
    void setTextData(QString title, QString addInfo);

    void show();        // block parent method
    int  exec() const;

    Ui::MatrixView *ui;
};

#endif // MATRIXVIEW_H
