#include "myspinbox.h"

#include <QKeyEvent>

mySpinBox::mySpinBox(QWidget *&widget) :
    QSpinBox(widget)
{
}

mySpinBox::mySpinBox(QGroupBox *&groupbox) :
    QSpinBox(groupbox)
{
}

void mySpinBox::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Return:
        emit returnPressed();
    default:
        QSpinBox::keyReleaseEvent(event);
    }
}
