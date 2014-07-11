#include "mydirectpushbutton.h"
#include <QPainter>
#include <utility>

myDirectPushButton::myDirectPushButton(directions direct, QWidget *parent):
    QPushButton(parent)
{
    _direction = direct;

    connect(this, &myDirectPushButton::clicked,
            this, &myDirectPushButton::inverseDirection);
}

directions myDirectPushButton::direction()
{
    return _direction;
}

void myDirectPushButton::inverseDirection()
{
    if(_direction == directions::INPUT) {
        _direction = directions::OUTPUT;
    } else {
        _direction = directions::INPUT;
    }
    update();
}

void myDirectPushButton::setDirection(directions d)
{
    _direction = d;
}

void myDirectPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setPen( QPen(QBrush(Qt::black), 2) );
    painter.setRenderHint(QPainter::Antialiasing);

    int xEnd   = width() * 0.7;
    int xStart = width() * 0.3;
    int lineEnd = xEnd - width() * 0.15;

    if(_direction == directions::INPUT) {
        std::swap(xEnd, xStart);
        lineEnd = width() - lineEnd;
    }

    QPoint startPoint(xStart, height() * 0.5);
    QPoint endPoint(xEnd, height() * 0.5);
    QPoint a(lineEnd, height() * 0.25);
    QPoint b(lineEnd, height() * 0.75);

    painter.drawLine(startPoint, endPoint);
    painter.drawLine(endPoint, a);
    painter.drawLine(endPoint, b);
}
