#ifndef MYCOLORPUSHBUTTON_H
#define MYCOLORPUSHBUTTON_H

#include <QPushButton>

class myColorButtonDialog : public QPushButton
{
    Q_OBJECT
public:
    explicit myColorButtonDialog(QWidget *parent = 0);
    explicit myColorButtonDialog(QColor defaultColor, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void newColorSelected(QColor color);

public slots:
    void setColor(QColor color);

private:
    QColor _curColor;

    const int marginHeightSize = 5;
    const int marginWidthSize = 6;

private slots:
    void colorDialogSlot();
};

#endif // MYCOLORPUSHBUTTON_H
