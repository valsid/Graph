#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QContextMenuEvent>
#include <QMenu>

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphView(QWidget *parent = 0);

public slots:

    void action_addIsolatedVertex();
//    void action_addToSelect();
    void action_removeSelect();

signals:
    void removeSelectedVertex();
    void addIsolatedVertex(QPointF pos);

protected:
    void mousePressEvent  (QMouseEvent *event);
    void wheelEvent       (QWheelEvent *event);
    void keyPressEvent    (QKeyEvent *event);
    void keyReleaseEvent  (QKeyEvent *event);
    void contextMenuEvent (QContextMenuEvent *event);

private:
    void zoom(qreal r);
    void processPressedKeys();

    QPointF menuExeScenecPos;
    QMenu *_menu;
    qreal curZoom = 1;
    QMap<Qt::Key, bool> _pressedKeys;
};

#endif // GRAPHVIEW_H
