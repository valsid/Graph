#include "controls.h"
#include "ui_controls.h"

#include "structures/exceptions.h"
#include "structures/comparators.h"

Controls::Controls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Controls),
    _currentSelectVertex(nullptr)
{
    ui->setupUi(this);
}

Controls::~Controls()
{
    clearNodeListItems();
    delete ui;
}

void Controls::vertexSelected(Vertex *v)
{
    ui->stackedWidget->setCurrentIndex(1);
    _currentSelectVertex = v;
}

void Controls::selectedVertexMoved(Vertex *v)
{
    if(v != _currentSelectVertex) {
        vertexSelected(v);
    }

    ui->sFixedPosCheckBox->setEnabled(v->graphicsVertex()->isFixedPos());

    ui->sFixedPosXSpinBox->setValue(v->graphicsVertex()->x());
    ui->sFixedPosYSpinBox->setValue(v->graphicsVertex()->y());
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

    data.vertices.reserve(edgeList.size());

    edgeData tmp;
    for(edgeListRowData *row : edgeList.values()) {
        tmp.vertexId   = row->vertexId;
        tmp.edgeWeight = row->weight;
        tmp.direction  = row->direction.direction();
        data.vertices.append(tmp);
    }

    qSort(data.vertices.begin(), data.vertices.end(), Comparators());

    emit newNode(data);
}

void Controls::on_nodeListAddButton_clicked()
{
    int vertexId     = ui->nodeListSpinBox->value();

    if(!edgeList.contains(vertexId)) {
        edgeList[vertexId] = new edgeListRowData(vertexId, ui->nodeListWeightSpinBox->value(),
                                                 ui->nodeListSpinBox->text(),
                                                 ui->nodeListWeightSpinBox->text());
        addEdgeToView(edgeList[vertexId]);
    }
}

void Controls::on_nodeListDelButton_clicked()
{
    edgeListRowData *r = edgeList.take(ui->nodeListSpinBox->value());

    r->direction.setParent(nullptr);

    delete r->widgetLink;
    delete r;
}

void Controls::on_nodeListClearButton_clicked()
{
//    _vertices->clear();
//    _verticesWeight->clear();
//    _verticesDirect.clear();

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

void Controls::on_backToAddVertex_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void Controls::updateDataFromSelectedVertex()
{
    GraphicVertex *g = _currentSelectVertex->graphicsVertex();

    ui->sFixedPosCheckBox->setEnabled(g->isFixedPos());

    ui->sFixedPosXSpinBox->setValue(g->x());
    ui->sFixedPosYSpinBox->setValue(g->y());
}

void Controls::clearNodeListItems()
{
    for(edgeListRowData *d : edgeList.values()) {
        d->direction.setParent(nullptr);
        delete d->widgetLink;
        delete d;
    }

    edgeList.clear();
}

void Controls::addEdgeToView(edgeListRowData *row)
{
    QWidget     *widget     = new QWidget(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(widget);
    hBoxLayout->setMargin(0);

    QLabel *t = new QLabel(row->textVertexId, widget);
    t->setAlignment(Qt::AlignCenter);
    hBoxLayout->addWidget(t);

    t = new QLabel(row->textWeight, widget);
    t->setAlignment(Qt::AlignCenter);
    hBoxLayout->addWidget(t);

    hBoxLayout->addWidget(&row->direction);

    row->widgetLink = widget;
    ui->nodeListDataLayout->addWidget(widget);
}
