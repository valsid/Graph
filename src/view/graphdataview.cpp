#include "graphDataView.h"
#include "ui_matrixview.h"

#include "matrixitemmodel.h"
#include "pathsItemModel.h"

const int stackedWidgetMatrixTableViewIndex = 1;
const int stackedWidgetTextViewIndex = 0;

QString numberToAlignStringAndInf(int number, int width = 0) {
    QString result;
    QString num = (number == INF) ? QApplication::tr("∞") : QString::number(number);

    int n = width - num.size();

    for(int i = 0; i < n; i++) {
        result.append(" ");
    }

    result.append(num);

    return result;
}

MatrixView::MatrixView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MatrixView)
{
    ui->setupUi(this);
}

MatrixView::~MatrixView()
{
    delete ui;
}

void MatrixView::showMatrix(graphMatrix *matrix, QString title, QString addInfo)
{
    MatrixItemModel model(matrix, this);
    showTable(&model, title, addInfo);
}


void MatrixView::showVertexList(QVector<int> *vertices, QString title, QString addInfo)
{
    ui->ViewStackedWidget->setCurrentIndex(stackedWidgetTextViewIndex);
    setTextData(title, addInfo);

    ui->matrixTextBrowser->clear();

    for(int vertexID : *vertices) {
        ui->matrixTextBrowser->append(QString::number(vertexID));
    }


    QDialog::exec();
}

void MatrixView::showVertexDegree(QVector<degree> *degreeV, QString title, QString addInfo)
{
    ui->ViewStackedWidget->setCurrentIndex(stackedWidgetTextViewIndex);
    setTextData(title, addInfo);

    ui->matrixTextBrowser->clear();


    QString rowStr;
    for(degree d : *degreeV) {
        rowStr = QString::number(d.vertexID) + tr(": ") + QString::number(d.vertexDegree);

        ui->matrixTextBrowser->append(rowStr);
    }

    QDialog::exec();
}

void MatrixView::showManyVertexList(const QVector<QVector<int> > &pathList, QString title, QString addInfo)
{
    ui->ViewStackedWidget->setCurrentIndex(stackedWidgetTextViewIndex);
    setTextData(title, addInfo);

    ui->matrixTextBrowser->clear();

    QString rowStr;
    for(QVector<int> rowList : pathList) {
        for(int vertexId : rowList) {
            rowStr.append(QString::number(vertexId) + " - ");
        }
        rowStr.resize(rowStr.size() - 3);
        ui->matrixTextBrowser->append(rowStr);
        rowStr.clear();
    }

    QDialog::exec();
}

void MatrixView::showSimpleString(const QString &string, QString title, QString addInfo)
{
    ui->ViewStackedWidget->setCurrentIndex(stackedWidgetTextViewIndex);
    setTextData(title, addInfo);

    ui->matrixTextBrowser->clear();

    ui->matrixTextBrowser->append(string);

    QDialog::exec();
}

//#include <QItemDelegate>
//class ItemDelegate : public QItemDelegate
//{
//public:
//  ItemDelegate()
//  {}
//  QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
//  {
//    return QSize(50,50);
//  }
//};

void MatrixView::showTable(QAbstractItemModel *model, QString title, QString addInfo)
{
    ui->ViewStackedWidget->setCurrentIndex(stackedWidgetMatrixTableViewIndex);
    setTextData(title, addInfo);

    ui->matrixTableView->setModel(model);
//    ItemDelegate *delegate = new ItemDelegate();
//    ui->matrixTableView->setItemDelegate(delegate);

    QDialog::exec();
}

void MatrixView::showGraphConnectivity(const graphConnectivity &connectivity, QString title, QString addInfo)
{
    QString connectType;
    switch (connectivity) {
    case graphConnectivity::notConnectivity:
        connectType = "Граф незв’язний";
        break;
    case graphConnectivity::STRONGLY:
        connectType = "Граф сильнозв’язний";
        break;
    case graphConnectivity::WEAKLY:
        connectType = "Граф слабкозв’язний";
        break;
    case graphConnectivity::SIDED:
        connectType = "Граф однобічно-зв’язний";
        break;
    }

    showSimpleString(connectType, title, addInfo);
}

#include <QMessageBox>

void MatrixView::showPathToAll(const pathToAll &paths, QString title, QString addInfo)
{
    QAbstractItemModel *model = new pathsItemModel(paths, this);

    if(paths.isHaveNegativeCycles) {
        QMessageBox warningMessage;
        warningMessage.setWindowTitle(tr("Від'ємний цикл"));
        warningMessage.setIcon(QMessageBox::Warning);
        warningMessage.setText(tr("Граф має цикл від'ємної ваги"));
        warningMessage.exec();
        return;
    }

    if(paths.isHaveNegativeEdgeWeight) {
        QMessageBox warningMessage;
        warningMessage.setWindowTitle(tr("Від'ємне ребро"));
        warningMessage.setIcon(QMessageBox::Warning);
        warningMessage.setText(tr("Граф має ребро від'ємної ваги"));
        warningMessage.exec();
        return;
    }

    showTable(model, title, addInfo);

    /*
    const int minNumberWidth = 2;

    QString text;
    text.append(tr("Найменший шлях до всіх інших вершин від вершини ") + QString::number(paths.fromVertexId) + tr("\n"));

    for(weightedPath path: paths.paths.values()) {
        text.append(tr("Вершина: ") + numberToAlignStringAndInf(path.id, minNumberWidth)
                       + tr(". Відстань: ") + numberToAlignStringAndInf(path.weight, minNumberWidth)
                       + tr(". Щлях: "));
        for(int id : path.path) {
            text.append(numberToAlignStringAndInf(id, minNumberWidth) + " - ");
        }
        text.resize(text.size() - 3);
        text.append(tr("\n"));
    }


    showSimpleString(text, title, addInfo);
    */
}

void MatrixView::on_OKButton_clicked()
{
    this->close();
}

void MatrixView::setTextData(QString title, QString addInfo)
{
    setWindowTitle(title);
    ui->titleLabel->setText(title);

    if(addInfo != "") {
        ui->infoLabel->setText(addInfo);
    } else {
        ui->infoLabel->setVisible(false);
    }
}

