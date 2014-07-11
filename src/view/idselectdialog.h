#ifndef IDSELECTDIALOG_H
#define IDSELECTDIALOG_H

#include <QDialog>
#include "../graph/graph.h"

namespace Ui {
class idSelectDialog;
}

class idSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit idSelectDialog(const Graph &g, QWidget *parent = 0);
    ~idSelectDialog();

    int lastSelectedVertexId() const;

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    int _lastSelectedVertexId = -1;
    const Graph &_graph;
    Ui::idSelectDialog *ui;
};

#endif // IDSELECTDIALOG_H
