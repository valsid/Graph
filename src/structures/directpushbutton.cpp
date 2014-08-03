#include "directpushbutton.h"
#include <QPainter>
#include <utility>

DirectPushButton::DirectPushButton(QWidget *parent):
    DirectPushButton(directions::OUTPUT, parent)
{ }

DirectPushButton::DirectPushButton(directions direct, QWidget *parent):
    QPushButton(parent)
{
    _direction = direct;

    connect(this, &DirectPushButton::clicked,
            this, &DirectPushButton::inverseDirection);
}

directions DirectPushButton::direction()
{
    return _direction;
}

void DirectPushButton::inverseDirection()
{
    if(_direction == directions::INPUT) {
        _direction = directions::OUTPUT;
    } else {
        _direction = directions::INPUT;
    }
    update();
}

void DirectPushButton::setDirection(directions d)
{
    _direction = d;
}

void DirectPushButton::paintEvent(QPaintEvent *event)
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
