#ifndef MYDIRECTPUSHBUTTON_H
#define MYDIRECTPUSHBUTTON_H

#include <QPushButton>
#include "nodeData.h"

class myDirectPushButton : public QPushButton
{
public:
    myDirectPushButton(directions direct, QWidget *parent = 0);

    directions direction();

public slots:
    void inverseDirection();
    void setDirection(directions d);

protected:
    void paintEvent(QPaintEvent *event);
    directions _direction;
};

#endif // MYDIRECTPUSHBUTTON_H
