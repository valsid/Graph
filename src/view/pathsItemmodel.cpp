#include "pathsItemModel.h"

#include <QDebug>

pathsItemModel::pathsItemModel(const pathToAll &paths, QObject *parent) :
    QAbstractItemModel(parent),
    _paths(paths)
{
    _keys = paths.keys;
    qSort(_keys);
}

QModelIndex pathsItemModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex pathsItemModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return *(new QModelIndex());
}

int pathsItemModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _keys.size();
}

int pathsItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return colNumber;
}

QVariant pathsItemModel::data(const QModelIndex &index, int role) const
{

    if (index.row() < 0 || index.row() >= _keys.size() ||
            index.column() < 0 || index.column() >= colNumber)   {
        return QVariant();
    }

    if(role == Qt::DisplayRole) {
        int row = _keys.at(index.row());

        switch(index.column()) {
        case 0:
            return row;
            break;
        case 1:
            if(_paths.paths.contains(row)) {
                return _paths.paths[row].weight;
            } else {
                return tr("∞");
            }
            break;
        case 2:
            QString text;

            for(int id : _paths.paths[row].path) {
                text.append(QString::number(id) + tr(" - "));
            }
            text.resize(text.size() - 3);
            text.append(tr("\n"));

            return text;
            break;
        }

        return QVariant();
    } else if(role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignCenter);
    } else {
        return QVariant();
    }
}

QVariant pathsItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole) {
        return QVariant();
    }

    if(orientation == Qt::Horizontal) {
        if(section == 0) {
            return tr("Вершина: ");
        }
        if(section == 1) {
            return tr("Відстань: ");
        }
        if(section == 2) {
            return tr("Шлях: ");
        }
        return QVariant();
    } else {
        return QString::number(_keys.at(section));
    }
}
