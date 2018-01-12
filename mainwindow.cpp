#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <qwt_legend.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_newFile,SIGNAL(triggered()),this,SLOT(doProcessTriggeredByCreateFile()));
    connect(ui->action_serialportSet,SIGNAL(triggered()),this,SLOT(doProcessTriggeredByCommunicationSet()));
    connect(ui->action_about,SIGNAL(triggered()),this,SLOT(doProcessTriggeredByAbout()));
    connect(ui->action_sensor_check,SIGNAL(triggered()),this,SLOT(doProcessTriggeredBySensorCheck()));
    connect(ui->action_test_object,SIGNAL(triggered()),this,SLOT(doProcessTriggeredByTestObject()));
    connect(ui->action_life_test,SIGNAL(triggered()),this,SLOT(doProcessTriggeredByLifeTest()));

    connect(&_communication_set,SIGNAL(OpenSerialClicked(QString,QString,unsigned int)),
            this,SIGNAL(OpenSerialClicked(QString,QString,unsigned int)));
    connect(&_test_object,SIGNAL(SendCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t)),
            this,SLOT(onSaveTestObjectConf(QStringList,uint8_t,uint8_t,uint32_t,uint32_t)));
    connect(ui->btn_begin_mesurement,SIGNAL(clicked()),this,SLOT(onSendTestObjectConf()));

    InitPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitPlot()
{
    _myPlot = new QwtPlot(this);
    _myPlot->setGeometry(450,60,700,550);
    _curve_name = "swicth mechanical characteristics curve";
    _myPlot->setTitle(_curve_name);
    _myPlot->setCanvasBackground(Qt::gray);

    _myPlot->setAxisTitle(QwtPlot::xBottom, "x value, time");
    _myPlot->setAxisTitle(QwtPlot::yLeft,"y value");
    _myPlot->setAxisScale(QwtPlot::yLeft,0,4,0.5);
    _myPlot->setAxisScale(QwtPlot::xBottom, 0,4,0.5);

    _grid = new QwtPlotGrid();
    _grid->enableX(true);
    _grid->enableY(true);
    _grid->setMajorPen(Qt::black,0,Qt::DotLine);
    _grid->attach(_myPlot);

    _zoomer = new QwtPlotZoomer(_myPlot->canvas());
    _zoomer->setRubberBandPen(QColor(Qt::blue));
    _zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton,Qt::ControlModifier);
    _zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton);

    _myPlot->show();
}

void MainWindow::InitCreateFile(QString default_path)
{
    _create_file.SetDefaultPath(default_path);
}

void MainWindow::InitSerialAttribute(QString serial_port)
{
    _communication_set.SetSerialPort(serial_port);
}

void MainWindow::InitLifeTest(QString test_time, QString interval_time)
{
    _life_test.InitLifeTest(test_time,interval_time);
}

void MainWindow::InitSensorCheck(QString current_coefficient, QString voltage_coefficient, QString acceleration_coefficient)
{
    _sensor_check.InitSensorCheck(current_coefficient,voltage_coefficient,acceleration_coefficient);
}

void MainWindow::InitTestObject(QString sampling_freq, QString sampling_cycle, QString fracture_before, QString fracture_after)
{
    _test_object.InitTestObject(sampling_freq,sampling_cycle,fracture_before,fracture_after);
}

void MainWindow::doProcessTriggeredByCreateFile()
{
    _create_file.show();
}

void MainWindow::doProcessTriggeredByCommunicationSet()
{
    _communication_set.show();
}

void MainWindow::doProcessTriggeredBySensorCheck()
{
    _sensor_check.show();
}

void MainWindow::doProcessTriggeredByTestObject()
{
    _test_object.show();
}

void MainWindow::doProcessTriggeredByLifeTest()
{
    _life_test.show();
}

void MainWindow::doProcessTriggeredByAbout()
{
    _about.show();
}

void MainWindow::onSaveTestObjectConf(QStringList name_list, uint8_t name_number_min, uint8_t name_number_max,
                                      uint32_t range_min, uint32_t range_max)
{
    _name_list = name_list;
    _curve_start = name_number_min;
    _curve_end = name_number_max;
    _range_start = range_min;
    _range_end = range_max;
}

void MainWindow::onSendTestObjectConf()
{
    emit SendCollectDataPara(_name_list,_curve_start,_curve_end,_range_start,_range_end);
}

double MainWindow::GetYMax(double new_value,double old_value)
{
    return new_value > old_value ? new_value : old_value;
}

double MainWindow::GetYMin(double new_value,double old_value)
{
    return new_value < old_value ? new_value : old_value;
}

void MainWindow::DataToDraw(const QString data_name, const QVector<double> data)
{
    std::cout << "The number of data that is drawing in mainwindow is : " << data.size() << std::endl;
    for(QVector<double>::const_iterator it = data.begin();it != data.end();++it)
    {
        std::cout << *it << " " << std::endl;
    }
    std::cout << std::endl;

    QVector<double> x;
    double x_min = 0;
    double x_max = 9;
    //double x_max = _test_object.GetRangeUpper();

    const double *y_new_max = std::max_element(&data[0], &data[0]+data.size());
    const double *y_new_min = std::min_element(&data[0], &data[0]+data.size());

    _y_max = GetYMax(*y_new_max,_y_max);
    _y_min = GetYMin(*y_new_min,_y_min);

    if((_y_max - _y_min) < EPSINON)
    {
       _myPlot->setAxisScale(QwtPlot::yLeft,0,_y_max,_y_max/10);
    }
    else
    {
       _myPlot->setAxisScale(QwtPlot::yLeft,_y_min,_y_max,(_y_max - _y_min)/10);
    }

    for(double i = x_min; i <= x_max; ++i)
    {
        x.push_back(i);
    }

    _myPlot->setAxisScale(QwtPlot::xBottom, x_min,x_max,(x_max - x_min)/10);
    GetCurve(data_name)->setSamples(x,data);
    if(data_name == tr("A相断口"))
    {
        GetCurve(data_name)->setPen(Qt::red,2);
    }
    else if(data_name == tr("B相断口"))
    {
        GetCurve(data_name)->setPen(Qt::yellow,2);
    }
    else if(data_name == tr("C相断口"))
    {
        GetCurve(data_name)->setPen(Qt::green,2);
    }
    else if(data_name == tr("位移量"))
    {
        GetCurve(data_name)->setPen(Qt::blue,2);
    }
    else if(data_name == tr("分合闸线圈电流"))
    {
        GetCurve(data_name)->setPen(Qt::black,2);
    }
    _myPlot->replot();
}

QwtPlotCurve *MainWindow::GetCurve(const QString &curve_name)
{
    if (!_curves.contains(curve_name))  {
        _curves[curve_name] = new QwtPlotCurve(curve_name);
        _curves[curve_name]->setTitle(curve_name);
        _curves[curve_name]->attach(_myPlot);
        _myPlot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);
    }
    return _curves[curve_name];
}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit closed();
}
