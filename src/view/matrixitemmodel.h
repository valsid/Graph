#ifndef MATRIXITEMMODEL_H
#define MATRIXITEMMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include "../algorithm/algorithms.h"

class MatrixItemModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit MatrixItemModel(const graphMatrix *matrix, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
signals:

public slots:

private:
    const QVector <int> rowKeys;
    const QVector <int> colKeys;
    const simpleMatrix _matrix;
};

#endif // MATRIXITEMMODEL_H
