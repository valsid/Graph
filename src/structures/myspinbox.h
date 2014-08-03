#ifndef MYSPINBOX_H
#define MYSPINBOX_H

#include <QSpinBox>
#include <QGroupBox>

class mySpinBox : public QSpinBox
{
    Q_OBJECT
public:
    mySpinBox(QWidget *widget);

signals:
    void returnPressed();

public slots:
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MYSPINBOX_H
