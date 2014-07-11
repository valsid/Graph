#include "settings.h"
#include "ui_settings.h"

#include "../globalsettings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    GlobalSettings *t = new GlobalSettings(this);

    connect(ui->okButton, &QPushButton::clicked,
            this,         &Settings::close);

    ui->colorVertexButton->setColor(GlobalSettings::vertexDefaultPen().color());
    ui->colorEdgeButton  ->setColor(GlobalSettings::edgeDefaultPen().color());
    ui->colorSelectVertexButton->setColor(GlobalSettings::vertexSelectPen().color());
    ui->colorSelectEdgeButton  ->setColor(GlobalSettings::edgeSelectPen().color());

    connect(ui->colorVertexButton, &myColorButtonDialog::newColorSelected,
            t,                     &GlobalSettings::setVertexDefaultPen);
    connect(ui->colorEdgeButton,   &myColorButtonDialog::newColorSelected,
            t,                     &GlobalSettings::setEdgeDefaultPen);
    connect(ui->colorSelectVertexButton, &myColorButtonDialog::newColorSelected,
            t,                           &GlobalSettings::setVertexSelectPen);
    connect(ui->colorSelectEdgeButton,   &myColorButtonDialog::newColorSelected,
            t,                           &GlobalSettings::setEdgeSelectPen);


    void (QLCDNumber:: *display)(int) = &QLCDNumber::display;   // slot

    connect(ui->horizontalSlider, &QSlider::valueChanged,
            ui->lcdNumber_2,        display);

    connect(ui->horizontalSlider_2, &QSlider::valueChanged,
            ui->lcdNumber,          display);

    initSettings();
}

Settings::~Settings()
{
    delete ui;
}


void Settings::on_horizontalSlider_sliderMoved(int position)
{
    GlobalSettings::setForce1(position);
}

void Settings::on_horizontalSlider_2_sliderMoved(int position)
{
    GlobalSettings::setForce2(ui->horizontalSlider_2->maximum() - position);
}

void Settings::on_checkBox_2_clicked(bool checked)
{
    GlobalSettings::setShowEdgeId(checked);
}

void Settings::on_checkBox_clicked(bool checked)
{
    GlobalSettings::setShowVertexId(checked);
}

void Settings::on_checkBox_3_clicked(bool checked)
{
    GlobalSettings::setShowEdgeWeight(checked);
}

void Settings::on_forceCheckBox_clicked(bool checked)
{
    GlobalSettings::setUsingForce(checked);
}

void Settings::initSettings()
{
    ui->checkBox->setChecked(GlobalSettings::isShowVertexId());
    ui->checkBox_2->setChecked(GlobalSettings::isShowEdgeId());
    ui->checkBox_3->setChecked(GlobalSettings::isShowEdgeWeight());
    ui->forceCheckBox->setChecked(GlobalSettings::isUsingForce());

    ui->lcdNumber->display(GlobalSettings::force2());
    ui->lcdNumber_2->display(GlobalSettings::force1());

    ui->colorSelectEdgeButton->setColor(GlobalSettings::edgeSelectPen().color());
    // ...
}
