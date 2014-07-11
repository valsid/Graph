#include "matrixitemmodel.h"

MatrixItemModel::MatrixItemModel(const graphMatrix *matrix, QObject *parent) :
    QAbstractTableModel(parent),
    rowKeys(matrix->rowKeys),
    colKeys(matrix->colKeys),
    _matrix(matrix->matrix)
{ }

int MatrixItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rowKeys.size();
}

int MatrixItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return colKeys.size();
}

QVariant MatrixItemModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowKeys.size() ||
            index.column() < 0 || index.column() >= colKeys.size())   {
        return QVariant();
    }

    switch (role) {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignCenter);
        break;
    case Qt::DisplayRole:
    {
        int row = rowKeys.at(index.row());
        int col = colKeys.at(index.column());

        if( _matrix[row].contains(col) ) {
            return _matrix[row][col];
        } else {
            return tr("∞");
        }
    }
        break;
    }
    return QVariant();
}

QVariant MatrixItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role) {
    case Qt::DisplayRole:
        if(orientation == Qt::Horizontal) {
            return QVariant(colKeys.at(section));
        } else {
            return QVariant(rowKeys.at(section));
        }
    default:
        return QAbstractTableModel::headerData(section, orientation, role);
    }
}
