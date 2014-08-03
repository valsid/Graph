#include "colorpushbutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QColorDialog>

#include <QDebug>

myColorButtonDialog::myColorButtonDialog(QWidget *parent) :
    QPushButton(parent)
{
    setAutoFillBackground(true);
    setFlat(true);

    _curColor = Qt::white;

    connect(this, &myColorButtonDialog::clicked,
            this, &myColorButtonDialog::colorDialogSlot);

    connect(this, &myColorButtonDialog::newColorSelected,
            this, &myColorButtonDialog::setColor);
}

myColorButtonDialog::myColorButtonDialog(QColor defaultColor, QWidget *parent) :
    QPushButton(parent)
{
    setAutoFillBackground(true);
    setFlat(true);
    if(defaultColor.isValid()) {
        _curColor = defaultColor;
    }

    connect(this, &myColorButtonDialog::clicked,
            this, &myColorButtonDialog::colorDialogSlot);

    connect(this, &myColorButtonDialog::newColorSelected,
            this, &myColorButtonDialog::setColor);
}

void myColorButtonDialog::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    painter.setBrush(QBrush(_curColor));

//    QRect rect = this->rect();
    QRect rect;
    rect.setX(marginWidthSize);
    rect.setY(marginHeightSize);
    rect.setWidth (width()  - marginWidthSize  * 2);
    rect.setHeight(height() - marginHeightSize * 2);

    painter.drawRect(rect);
}

void myColorButtonDialog::colorDialogSlot()
{
    emit newColorSelected( QColorDialog::getColor() );
}

void myColorButtonDialog::setColor(QColor color)
{
    if(color.isValid()) {
        _curColor = color;
    }
}
