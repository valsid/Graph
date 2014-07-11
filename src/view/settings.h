#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_checkBox_2_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_forceCheckBox_clicked(bool checked);

private:
    void initSettings();
    Ui::Settings *ui;
};

#endif // SETTINGS_H
