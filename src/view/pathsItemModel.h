#ifndef PATHSITEMMODEL_H
#define PATHSITEMMODEL_H

#include <QAbstractItemModel>
#include "../algorithm/algorithms.h"

class pathsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit pathsItemModel(const pathToAll &paths, QObject *parent = 0);

    QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

signals:

public slots:

private:
    QVector <int>     _keys;
    const pathToAll _paths;

    const int colNumber = 3;
};

#endif // PATHSITEMMODEL_H
