#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "about.h"
#include "create_file.h"
#include "communication_set.h"
#include "sensor_check.h"
#include "test_object.h"
#include "life_test.h"
#include "process_data_thread.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitCreateFile(QString default_path);
    void InitSerialAttribute(QString serial_port);
    void InitLifeTest(QString test_time,QString interval_time);
    void InitSensorCheck(QString current_coefficient,QString voltage_coefficient,QString acceleration_coefficient);
    void InitTestObject(QString sampling_freq,QString sampling_cycle,QString fracture_before,QString fracture_after);

signals:
    void closed();
    void OpenSerialClicked(QString protocol,QString serial_port,unsigned int baud_rate);
    void SendCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t);
    void BeginMesurementClicked();

public slots:
    void DataToDraw(const QString,const QVector<double>);

private slots:
    void doProcessTriggeredByCreateFile();
    void doProcessTriggeredByCommunicationSet();
    void doProcessTriggeredByAbout();
    void doProcessTriggeredBySensorCheck();
    void doProcessTriggeredByTestObject();
    void doProcessTriggeredByLifeTest();

    void onSaveTestObjectConf(QStringList,uint8_t,uint8_t,uint32_t,uint32_t);
    void onSendTestObjectConf();

private:
    void InitPlot();
    QwtPlotCurve *GetCurve(const QString &curve_name);
    double GetYMax(double new_value,double old_value);
    double GetYMin(double new_value,double old_value);

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::MainWindow *ui;

    const static double EPSINON = 0.001;

    CreateFile _create_file;
    CommunicationSet _communication_set;
    SensorCheck _sensor_check;
    TestObject _test_object;
    LifeTest _life_test;
    About _about;

    //about qwt
    QwtPlot *_myPlot;
    QwtPlotCurve *_curve;
    QwtPlotGrid *_grid;
    QwtPlotZoomer *_zoomer;
    QString _curve_name;
    QMap<QString, QwtPlotCurve*> _curves;

    //for configure
    QStringList _name_list;
    uint8_t _curve_start;
    uint8_t _curve_end;
    uint32_t _range_start;
    uint32_t _range_end;

    //for axis
    double _y_max;
    double _y_min;
};

#endif // MAINWINDOW_H
