#ifndef CUSTOM_PROTOCOL_H
#define CUSTOM_PROTOCOL_H

#include "serialport.h"
#include "data_cache.h"
#include "inttypes.h"
#include <map>
#include <QObject>
#include <QMutex>
#include <QVector>
#include <QStringList>

class CustomProtocol: public QObject
{
    Q_OBJECT

public:
    CustomProtocol();
    bool Init();
    void OverseeSrialport();
    void Stop();
    bool IsRunning() const;
    void OpenSerialPort(QString serial_name,unsigned int baud_rate);

private:
    void ParseFrame();
    void DeleteFrame(QVector<char>::iterator begin,QVector<char>::iterator end);
    void JudgeFrameType();
    void SendSetFrametoServer(QStringList curve_list,uint8_t curve_start,uint8_t curve_end,uint32_t range_start,uint32_t range_end);
    void SendFrameToServer(const char id);
    void WriteToSerial();
    void SaveData();
    void SendDataToDraw();
    uint16_t CheckSum();
    uint16_t CaculateCheckSumToServer(unsigned int begin,unsigned int end);

signals:
    void DataToDraw(const QString,const QVector<double>);
    void DataProcessStart();

private slots:
    void onRecieveCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t);

private:
    DataCache _data_cache;
    mutable QMutex _lock;
    bool _running;
    SerialPort _serial_port;
    QString _serial_name;
    unsigned int _baud_rate;
    uint32_t _range_start;
    
    const static unsigned int _kMaxRecieveLenth = 1024;
    int _frame_current_num;
    QVector<char> _frame_data_from_server;
    QVector<char> _frame_data_to_server;
    QVector<double> _data;
    std::map<int,QString> _selected_data_name_map;
    std::map<QString,QVector<char> > _selected_data_chache_map;
    
    const static char ESTABLISH_CONNECTION = 0XA5;
    const static char CONFIRM_ESTABLISH_CONNECTION = 0XA1;
    const static char BEGIN_SAMPLING = 0XA7;
    const static char END_SAMPLING = 0XA6;
    const static char OBTAIN_DATA = 0XA0;
    const static char CONFIRM_LINK = 0XA8;
    const static char RESPOND_LINK = 0X55;
    const static char SEND_DATA = 0XA2;
    const static char STOP_DATA = 0XA3;
    const static char ADDR_LOW = 0XA5;
    const static char ADDR_HIGH = 0XA8;
    const static int FRAME_LENTH_MIN = 8;
};

#endif // CUSTOM_PROTOCOL_H
