#include "process_data_thread.h"
#include <QDebug>

ProcessDataThread::ProcessDataThread(QObject *parent) :
    QThread(parent)
{
    connect(this,SIGNAL(OpenSerialClicked(QString,QString,unsigned int)),this,SLOT(onChooseProtocol(QString,QString,unsigned int)));
}

void ProcessDataThread::onChooseProtocol(QString protocol, QString serial_port, unsigned int baud_rate)
{
    _protocol = protocol;
    _serial_port = serial_port;
    _baud_rate = baud_rate;

    Init();
}

void ProcessDataThread::Init()
{
    if(_protocol == tr("自定义协议"))
    {
        connect(this,SIGNAL(SendCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t)),
                &_custom_protocol,SLOT(onRecieveCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t)));
        connect(&_custom_protocol,SIGNAL(DataProcessStart()),this,SIGNAL(DataProcessStart()));
        connect(&_custom_protocol,SIGNAL(DataToDraw(const QString,const QVector<double>)),
                this,SIGNAL(DataToDraw(const QString,const QVector<double>)));
        _custom_protocol.OpenSerialPort(_serial_port,_baud_rate);
    }
    else if(_protocol == tr("协议B"))
    {
        //do nothing
    }
}

void ProcessDataThread::run()
{
    //启动传输协议,本处需要依据用户输出判断启动的协议类型
    if(_protocol == tr("自定义协议"))
    {
        if (!_custom_protocol.Init())
        {
            _custom_protocol.Stop();
            return;
        }
        qDebug() << "cunstom protocol int the run ";

        _custom_protocol.OverseeSrialport();
    }
    else if(_protocol == tr("协议B"))
    {
        //do nothing
    }
}

void ProcessDataThread::Stop()
{
    _custom_protocol.Stop();
}
