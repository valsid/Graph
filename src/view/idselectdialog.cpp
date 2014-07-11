#include "idselectdialog.h"
#include "ui_idselectdialog.h"

#include <QMessageBox>

idSelectDialog::idSelectDialog(const Graph &graph, QWidget *parent) :
    QDialog(parent),
    _graph(graph),
    ui(new Ui::idSelectDialog)
{
    ui->setupUi(this);
}

idSelectDialog::~idSelectDialog()
{
    delete ui;
}

int idSelectDialog::lastSelectedVertexId() const
{
    return _lastSelectedVertexId;
}

void idSelectDialog::on_okButton_clicked()
{
    if(_graph.isExistVertex( ui->spinBox->value() )) {
         _lastSelectedVertexId = ui->spinBox->value();
//         setResult(ui->spinBox->value());
         close();
    } else {
        QMessageBox warningMessage;
        warningMessage.setWindowTitle(tr("Обрана вершина не існує"));
        warningMessage.setIcon(QMessageBox::Warning);
        warningMessage.setText(tr("Обрана вершина не існує. Оберіть вірну вершину"));
        warningMessage.exec();
    }
}

void idSelectDialog::on_cancelButton_clicked()
{
//    setResult(-1);
    _lastSelectedVertexId = -1;
    close();
}
