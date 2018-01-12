#ifndef SENSOR_CHECK_H
#define SENSOR_CHECK_H

#include <QDialog>

namespace Ui {
class SensorCheck;
}

class SensorCheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit SensorCheck(QWidget *parent = 0);
    ~SensorCheck();

    void InitSensorCheck(QString current_sensor_coefficient,
                         QString voltage_sensor_coefficient,
                         QString acceleration_sensor_coefficient);
    
private:
    Ui::SensorCheck *ui;
};

#endif // SENSOR_CHECK_H
