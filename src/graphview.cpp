#include "graphview.h"

#include <QMenu>
#include <QWheelEvent>

GraphView::GraphView(QWidget *parent) :
    QGraphicsView(parent)
{
    setMinimumHeight(200);
    setMinimumWidth(250);

// TODO: mouse icon
//    setDragMode(QGraphicsView::ScrollHandDrag);

   _menu = new QMenu(this);

    QAction *addIsolatedVertex = _menu->addAction("Додати ізольовану вершину");
//    QAction *addSelectAdj      = _menu->addAction("Додати суміжну з виділеними вершинами");
    QAction *removeSelected    = _menu->addAction("Видалити обрані вершини");

    connect(addIsolatedVertex, &QAction::triggered,
             this,             &GraphView::action_addIsolatedVertex);

//    connect(addSelectAdj,       &QAction::triggered,
//            this,               &GraphView::action_addToSelect);

    connect(removeSelected, &QAction::triggered,
            this,           &GraphView::removeSelectedVertex);

    setViewportUpdateMode(SmartViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton
            && scene() != nullptr
            && !scene()->selectedItems().isEmpty()) {
        return;
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphView::wheelEvent(QWheelEvent *event)
{
    if(_pressedKeys.value(Qt::Key_Control, false)) {
        if(event->delta() > 0) {
            zoom(1.1);
        } else {
            zoom(0.9);
        }
        return;
//    qDebug() << event->delta();
//    zoom(event->delta() / 100.0);
    }

    QGraphicsView::wheelEvent(event);
}

void GraphView::zoom(qreal r)
{
    curZoom *= r;
    if(curZoom < 0.01) {
        curZoom = 0.01;
        return;
    }
    if(curZoom > 200) {
        curZoom = 200;
        return;
    }

    scale(r, r);
}

void GraphView::processPressedKeys()
{
    if(_pressedKeys.value(Qt::Key_Control, false)) {
        if(_pressedKeys.value(Qt::Key_Plus)) {
            zoom(1.1);
        }
        if(_pressedKeys.value(Qt::Key_Minus)) {
            zoom(0.9);
        }
    }
}


void GraphView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Equal:
    case Qt::Key_Plus:
        _pressedKeys.insert(Qt::Key_Plus, true);
        break;
    case Qt::Key_Minus:
        _pressedKeys.insert(Qt::Key_Minus, true);
        break;
    case Qt::Key_Control:
        _pressedKeys.insert(Qt::Key_Control, true);
    }
    QGraphicsView::keyPressEvent(event);

    processPressedKeys();
}

void GraphView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Equal:
    case Qt::Key_Plus:
        _pressedKeys.remove(Qt::Key_Plus);
        break;
    case Qt::Key_Minus:
        _pressedKeys.remove(Qt::Key_Minus);
        break;
    case Qt::Key_Control:
        _pressedKeys.remove(Qt::Key_Control);
    }
    QGraphicsView::keyReleaseEvent(event);
}

void GraphView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    menuExeScenecPos = mapToScene(event->pos());
    _menu->exec(event->globalPos());
}

void GraphView::action_addIsolatedVertex()
{
    emit addIsolatedVertex(menuExeScenecPos);
}

//void GraphView::action_addToSelect()
//{

//}

void GraphView::action_removeSelect()
{
    emit removeSelectedVertex();
}
