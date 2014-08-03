#ifndef MYDIRECTPUSHBUTTON_H
#define MYDIRECTPUSHBUTTON_H

#include <QPushButton>
#include "nodeData.h"

#include <QDebug>

class DirectPushButton : public QPushButton
{
public:
    DirectPushButton(QWidget *parent = nullptr);
    DirectPushButton(directions direct, QWidget *parent = nullptr);

    directions direction();

public slots:
    void inverseDirection();
    void setDirection(directions d);

protected:
    void paintEvent(QPaintEvent *event);
    directions _direction;
};

#endif // MYDIRECTPUSHBUTTON_H
