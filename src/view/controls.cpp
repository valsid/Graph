#include "controls.h"
#include "ui_controls.h"

#include "structures/exceptions.h"
#include "structures/comparators.h"

Controls::Controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls),
    _vertices(new QList<int>),
    _verticesWeight(new QList<int>)
{
    ui->setupUi(this);
}

Controls::~Controls()
{
    delete ui;
}

void Controls::vertexSelected(vertexDataForSelection data)
{
    // TODO:
    return;

    ui->stackedWidget->setCurrentIndex(1);
    ui->sIdValueLabel->setText(QString::number(data.id));

    ui->sFixedPosCheckBox->setEnabled(data.isFixed);

    ui->sFixedPosXSpinBox->setValue(data.pos.x());
    ui->sFixedPosYSpinBox->setValue(data.pos.y());
}

void Controls::on_fixedPosCheckBox_toggled(bool checked)
{
    ui->fixedPosXspinBox->setEnabled(checked);
    ui->fixedPosYspinBox->setEnabled(checked);
}

void Controls::on_okButton_clicked()
{
    newNodeData data;
    data.isFixedPos     = ui->fixedPosCheckBox->isChecked();
    data.pos            = QPoint(ui->fixedPosXspinBox->value(), ui->fixedPosYspinBox->value());

    if(_vertices->size() != _verticesWeight->size()) {
        throw userException("Vertices list error");
    }
    int vertexNumber = _vertices->size();

    data.vertices.reserve(vertexNumber);

    edgeData tmp;
    for(int i = 0; i < vertexNumber; i++) {
        tmp.vertexId     = _vertices->at(i);
        tmp.vertexWeight = _verticesWeight->at(i);
        tmp.direction    = _verticesDirect.at(i)->direction();
        data.vertices.append(tmp);
    }

    qSort(data.vertices.begin(), data.vertices.end(), Comparators());

    emit newNode(data);
}

void Controls::on_nodeListAddButton_clicked()
{
    int nodeId     = ui->nodeListSpinBox->value();

    if(!_vertices->contains(nodeId)) {
        _vertices->append(nodeId);
        _verticesWeight->append( ui->nodeListWeightSpinBox->value() );

        addEdgesToList(ui->nodeListSpinBox->text(), ui->nodeListWeightSpinBox->text());
    }
}

void Controls::on_nodeListDelButton_clicked()
{
    int nodeId = ui->nodeListSpinBox->value();
    int index = _vertices->indexOf(nodeId);
    _vertices->removeAt(index);
    _verticesWeight->removeAt(index);

    clearNodeListItems();

    for(int i = 0; i < _vertices->size(); i++) {
        addEdgesToList(QString::number(_vertices->at(i)),
                       QString::number(_verticesWeight->at(i)));
    }
}

void Controls::on_nodeListClearButton_clicked()
{
    _vertices->clear();
    _verticesWeight->clear();
    _verticesDirect.clear();

    clearNodeListItems();
}

void Controls::on_nodeListSpinBox_returnPressed()
{
    on_nodeListAddButton_clicked();
}

void Controls::on_nodeListWeightSpinBox_returnPressed()
{
    on_nodeListAddButton_clicked();
    ui->nodeListSpinBox->setValue(ui->nodeListSpinBox->value() + 1);
}

void Controls::addEdgesToList(QString id, QString weight)
{
    QWidget     *widget  = new QWidget(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(widget);
    hBoxLayout->setMargin(0);

    QLabel *t = new QLabel(id, widget);
    t->setAlignment(Qt::AlignCenter);
    hBoxLayout->addWidget(t);

    t = new QLabel(weight, widget);
    t->setAlignment(Qt::AlignCenter);
    hBoxLayout->addWidget(t);

    // TODO: tmp
//    myDirectPushButton *tmp =;
    _verticesDirect.append( new myDirectPushButton(directions::INPUT, this));

    hBoxLayout->addWidget(_verticesDirect.last());

    ui->nodeListDataLayout->addWidget(widget);
}

void Controls::clearNodeListItems()
{
    QLayoutItem* item;
    auto layout   = ui->nodeListDataLayout;
    while ((item = layout->layout()->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
}

void Controls::on_backToAddVertex_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
