#include "sensor_check.h"
#include "ui_sensor_check.h"

SensorCheck::SensorCheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorCheck)
{
    ui->setupUi(this);
}

SensorCheck::~SensorCheck()
{
    delete ui;
}

void SensorCheck::InitSensorCheck(QString current_sensor_coefficient,
                                  QString voltage_sensor_coefficient,
                                  QString acceleration_sensor_coefficient)
{
    ui->current_coefficient->setText(current_sensor_coefficient);
    ui->voltage_coefficient->setText( voltage_sensor_coefficient);
    ui->acceleration_coefficient->setText(acceleration_sensor_coefficient);
}
