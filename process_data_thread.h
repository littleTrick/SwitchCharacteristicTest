#ifndef PROCESS_DATA_THREAD_H
#define PROCESS_DATA_THREAD_H

#include "custom_protocol.h"
#include <QThread>

class ProcessDataThread : public QThread
{
    Q_OBJECT
public:
    explicit ProcessDataThread(QObject *parent = 0);
    virtual void run();
    void Stop();
    
signals:
    void OpenSerialClicked(QString protocol,QString serial_port,unsigned int baud_rate);
    void SendCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t);
    void DataProcessStart();

    void DataToDraw(const QString,const QVector<double>);

private slots:
    void onChooseProtocol(QString,QString,unsigned int);

private:
    void Init();

private:
    CustomProtocol _custom_protocol;

    //for serial attrib and chosen protocol
    QString _protocol;
    QString _serial_port;
    unsigned int _baud_rate;
};

#endif // PROCESS_DATA_THREAD_H
