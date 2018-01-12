#include "custom_protocol.h"
#include "logging.h"
#include "errno.h"
#include <QDebug>
#include <QStringList>

CustomProtocol::CustomProtocol():
    _running(true)
{

}

bool CustomProtocol::Init()
{
    _serial_port.SetSerialPortName(_serial_name);
    _serial_port.SetSpeed(_baud_rate);

    std::cout << "=================================" << std::endl;
    std::cout << "serial name: " << _serial_name.toStdString()
              << "  buad rate: " << _baud_rate << std::endl;
    std::cout << "=================================" << std::endl;

    if(!_serial_port.Open())
    {
        LOG(ERROR) << "fail to open serial port";
        return false;
    }
    return true;
}

void CustomProtocol::OverseeSrialport()
{
    char buff[_kMaxRecieveLenth];
    ssize_t read_number = 0;
    std::cout << "while begin before " << std::endl;

    while(IsRunning())
    {
        read_number = _serial_port.Read(buff,sizeof(buff));
        if(read_number < 0)
        {
            LOG(ERROR) << "read error,and error number is: " << errno;
            break;
        }
        else if(read_number == 0)
        {
            LOG(INFO) << "read timeout";
        }
        else if(read_number > 0)
        {
            std::cout << "the number from serial is : " << read_number << std::endl;
            std::cout << "reading from serial: ";
            for(int i = 0; i < read_number; ++i)
            {
                unsigned char c = (unsigned char)buff[i];
                _frame_data_from_server.push_back(c);
                printf("%02X ",(unsigned char)buff[i]);
            }
            std::cout << std::endl;
            memset(buff,0,sizeof(buff));
            ParseFrame();
        }
    }
    LOG(INFO) << "data process thread exited";
}

void CustomProtocol::ParseFrame()
{
    if(_frame_data_from_server.size() < 8)
    {
        return;
    }

    if((_frame_data_from_server[0] != ADDR_LOW) || (_frame_data_from_server[1] != ADDR_HIGH))
    {
        //删除当前所存的无效报文
        //DeleteFrame(_frame_data_from_server.begin(),_frame_data_from_server.begin() + 1);
        DeleteFrame(_frame_data_from_server.begin(),_frame_data_from_server.end());
        return;
    }

    _frame_current_num = ((unsigned int)_frame_data_from_server[4] << 8) + (unsigned char)_frame_data_from_server[3];
    LOG(INFO) << "_frame_current_num : " << _frame_current_num;
    if(_frame_current_num + 7 > _frame_data_from_server.size())
    {
        //数据还未收完，返回继续收
        return;
    }
    else
    {
        uint16_t sum = ((unsigned int)(unsigned char)_frame_data_from_server[_frame_current_num+6] << 8)
                + (unsigned char)_frame_data_from_server[_frame_current_num+5];
        if(sum == CheckSum())
        {
            JudgeFrameType();

//            std::cout << "the number from serial is : " << (_frame_current_num + 7) << std::endl;
//            std::cout << "reading from serial: ";
//            for(int i = 0; i < (_frame_current_num + 7); ++i)
//            {
//                printf("%02X ",(unsigned char)_frame_data_from_server[i]);
//            }
//            std::cout << std::endl;

            DeleteFrame(_frame_data_from_server.begin(),_frame_data_from_server.begin()+ _frame_current_num + 7);
            LOG(INFO) << "the count of frame_data_server is :" << _frame_data_from_server.size();
        }
        else
        {
            DeleteFrame(_frame_data_from_server.begin(),_frame_data_from_server.begin() + _frame_current_num + 7);
        }
    }
}



void CustomProtocol::DeleteFrame(QVector<char>::iterator begin,QVector<char>::iterator end)
{
    //删除vector中所有报文
    _frame_data_from_server.erase(begin,end);
}

void CustomProtocol::JudgeFrameType()
{
    switch(_frame_data_from_server[2])                  //功能码
    {
        case ESTABLISH_CONNECTION:                      //接收到建立连接的报文
            SendFrameToServer(ESTABLISH_CONNECTION);
            break;
        case END_SAMPLING:                              //接收到采样结束的报文
            SendFrameToServer(OBTAIN_DATA);             //发送获取数据报文
            break;
        case CONFIRM_ESTABLISH_CONNECTION:              //确认链接建立
            SendFrameToServer(RESPOND_LINK);            //链路连通帧
            break;
        case SEND_DATA:                                 //收到携带数据的报文
            SaveData();                                 //存储报文
            SendFrameToServer(RESPOND_LINK);            //回复链路正常报文
            break;
        case CONFIRM_LINK:                              //测试链路
            SendFrameToServer(RESPOND_LINK);             //链路连通帧
            break;
        case STOP_DATA:
            SendFrameToServer(RESPOND_LINK);
            SendDataToDraw();
            break;
        default:
            LOG(WARN)<< "this is a invalid frame, and the ID is:"
                      << _frame_data_from_server[2];
    }

}

void CustomProtocol::onRecieveCollectDataPara(QStringList selected_data_name_list,uint8_t data_name_start,
                                              uint8_t data_name_end,uint32_t range_start,uint32_t range_end)
{
    for(int i = 0; i < selected_data_name_list.count(); ++i) //<1,A相断口>
    {
        if(selected_data_name_list.at(i) == tr("A相断口"))
        {
            _selected_data_name_map[1] = tr("A相断口");
        }
        else if(selected_data_name_list.at(i) == tr("B相断口"))
        {
            _selected_data_name_map[2] = tr("B相断口");
        }
        else if(selected_data_name_list.at(i) == tr("C相断口"))
        {
            _selected_data_name_map[3] = tr("C相断口");
        }
        else if(selected_data_name_list.at(i) == tr("位移量"))
        {
            _selected_data_name_map[4] = tr("位移量");
        }
        else if(selected_data_name_list.at(i) == tr("分合闸线圈电流"))
        {
            _selected_data_name_map[5] = tr("分合闸线圈电流");
        }
    }

    unsigned char *p_range_start = (unsigned char*)&range_start;
    unsigned char *p_range_end = (unsigned char*)&range_end;

    _range_start = range_start;
    unsigned int  begin_location = _frame_data_to_server.size(); //标记累加和起始位置
    _frame_data_to_server.push_back(0XA5);//0
    _frame_data_to_server.push_back(0XA8);
    _frame_data_to_server.push_back(0XA1);//2
    _frame_data_to_server.push_back(0X0A);
    _frame_data_to_server.push_back(0X00);//4
    _frame_data_to_server.push_back(*(p_range_start));
    _frame_data_to_server.push_back(*(p_range_start +1));//6
    _frame_data_to_server.push_back(*(p_range_start +2));
    _frame_data_to_server.push_back(*(p_range_start +3));//8
    _frame_data_to_server.push_back(*(p_range_end));
    _frame_data_to_server.push_back(*(p_range_end +1));//10
    _frame_data_to_server.push_back(*(p_range_end +2));
    _frame_data_to_server.push_back(*(p_range_end +3));//12
    _frame_data_to_server.push_back(data_name_start);
    _frame_data_to_server.push_back(data_name_end);//14

    unsigned int end_location = _frame_data_to_server.size();
    uint16_t sum = CaculateCheckSumToServer(begin_location,end_location);
    printf("begin: %02X ,end: %02X \n",begin_location,end_location);

    _frame_data_to_server.push_back(sum & 0X00FF);
    _frame_data_to_server.push_back((sum & 0XFF00) >> 8);//16

    WriteToSerial();
}

void CustomProtocol::SendFrameToServer(const char id)
{
    _frame_data_to_server.push_back(0XA5);
    _frame_data_to_server.push_back(0XA8);
    _frame_data_to_server.push_back(id);
    _frame_data_to_server.push_back(0X01);
    _frame_data_to_server.push_back(0X00);
    _frame_data_to_server.push_back(0XA5);
    unsigned short sum = 0X0A5 + 0X0A8 + (unsigned char)id + 0X01 + 0X0A5;
    _frame_data_to_server.push_back(sum & 0X00FF);
    _frame_data_to_server.push_back((sum & 0XFF00) >> 8);

    WriteToSerial();
}

void CustomProtocol::WriteToSerial()
{
    const char *buff = &_frame_data_to_server[0];
    ssize_t nwrite = _serial_port.Write(buff,_frame_data_to_server.size());
    if(nwrite == -1)
    {
#ifndef NDEBUG
        LOG(ERROR) << "write to serial is wrong ! and the error number is: " << errno;
#endif
        return;
    }
    else if(nwrite > 0)
    {
        std::cout << "the number to serial is : " << nwrite << std::endl;
        std::cout << "writing to serial: ";
        for(int i = 0; i < nwrite; ++i)
        {
             printf("%02X ",(unsigned char)_frame_data_to_server[i]);
        }
        std::cout << std::endl;

        //DeleteFrame(_frame_data_to_server.begin(),_frame_data_to_server.begin() + nwrite);
        _frame_data_to_server.erase(_frame_data_to_server.begin(),_frame_data_to_server.begin() + nwrite);
    }
}

void CustomProtocol::SaveData()
{
    unsigned char current_curve_id = _frame_data_from_server[5];//该条报文曲线编号 输出1
    std::map<int,QString>::iterator iter_curve = _selected_data_name_map.find(current_curve_id);//查找该曲线编号会否被选择
    if(iter_curve != _selected_data_name_map.end())
    {
        QString current_curve_name = iter_curve->second;//本条报文对应的曲线名称
        uint32_t data_offset = ((uint32_t)_frame_data_from_server[9] << 24) + ((uint32_t)_frame_data_from_server[8] << 16)
                                 + ((uint16_t)_frame_data_from_server[7] << 8) + ((uint8_t)_frame_data_from_server[6]); //数据偏移量
        //std::map<QString,QVector<char> >::iterator iter_data = _curve_data_map.find(current_curve_name);//该曲线对应的数据缓存
        if(current_curve_name == tr("A相断口"))
        {
            if((data_offset - _range_start) != _data_cache.AfractureSize())
            {
                return;                                             //数据发生重叠或者缺省
            }
            else
            {
                qDebug() << "I am saving data to A";
                for(int i = 0; i < _frame_current_num - 5; ++i)
                {
                    _data_cache.PushDataToAfracture(_frame_data_from_server[10 + i]);//10为数据位开始坐标
                }
            }
        }
        else if(current_curve_name == tr("B相断口"))
        {
            if((data_offset - _range_start) != _data_cache.BfractureSize())
            {
                return;                                             //数据发生重叠或者缺省
            }
            else
            {
                qDebug() << "I am saving data to B";
                for(int i = 0; i < _frame_current_num - 5; ++i)
                {
                    _data_cache.PushDataToBfracture(_frame_data_from_server[10 + i]);//10为数据位开始坐标
                }
            }
        }
        else if(current_curve_name == tr("C相断口"))
        {
            if((data_offset - _range_start) != _data_cache.CfractureSize())
            {
                return;                                             //数据发生重叠或者缺省
            }
            else
            {
                for(int i = 0; i < _frame_current_num - 5; ++i)
                {
                    _data_cache.PushDataToCfracture(_frame_data_from_server[10 + i]);//10为数据位开始坐标
                }
            }
        }
        else if(current_curve_name == tr("位移量"))
        {
            if((data_offset - _range_start) != _data_cache.DisplacementSize())
            {
                return;                                             //数据发生重叠或者缺省
            }
            else
            {
                for(int i = 0; i < _frame_current_num - 5; ++i)
                {
                    _data_cache.PushDataToDisplacement(_frame_data_from_server[10 + i]);//10为数据位开始坐标
                }
            }
        }
        else if(current_curve_name == tr("分合闸线圈电流"))
        {
            if((data_offset - _range_start) != _data_cache.CoilCurrentSize())
            {
                return;                                             //数据发生重叠或者缺省
            }
            else
            {
                for(int i = 0; i < _frame_current_num - 5; ++i)
                {
                    _data_cache.PushDataToCoilCurrent(_frame_data_from_server[10 + i]);//10为数据位开始坐标
                }
            }
        }

    }
    else
    {
        return;
    }
}

void CustomProtocol::SendDataToDraw()
{
    unsigned char current_curve_id = _frame_data_from_server[5];//该条报文曲线编号
    std::map<int,QString>::iterator iter_curve = _selected_data_name_map.find(current_curve_id);//查找该曲线编号会否被选择
    if(iter_curve != _selected_data_name_map.end())
    {
        QString current_curve_name = iter_curve->second;  //该条曲线名
        if(current_curve_name == tr("A相断口"))
        {
            QVector<char> Afracture = _data_cache.PopDataFromAfracture();
            for(unsigned int i = 0; i < _data_cache.AfractureSize(); ++i)
            {
                unsigned char c = static_cast<unsigned char>(Afracture[i]);
                float d = static_cast<float>(c);
                _data.push_back(d);
            }

            //for debug
            qDebug() << "the number of data A which is collected is: " << _data.size();
            for(QVector<double>::const_iterator it = _data.begin();it != _data.end();++it)
            {
                std::cout << *it << " " << std::endl;
            }
            std::cout << std::endl;

            emit DataToDraw(current_curve_name,_data);
            _data.erase(_data.begin(),_data.end());
        }
        if(current_curve_name == tr("B相断口"))
        {
            QVector<char> Bfracture = _data_cache.PopDataFromBfracture();
            for(unsigned int i = 0; i < _data_cache.BfractureSize(); ++i)
            {
                unsigned char c = static_cast<unsigned char>(Bfracture[i]);
                float d = static_cast<float>(c);
                _data.push_back(d);
            }

            //for debug
            qDebug() << "the number of data B which is collected is: " << _data.size();
            for(QVector<double>::const_iterator it = _data.begin();it != _data.end();++it)
            {
                std::cout << *it << " " << std::endl;
            }
            std::cout << std::endl;

            emit DataToDraw(current_curve_name,_data);
            _data.erase(_data.begin(),_data.end());
        }
        if(current_curve_name == tr("C相断口"))
        {
            QVector<char> Cfracture = _data_cache.PopDataFromCfracture();
            for(unsigned int i = 0; i < _data_cache.CfractureSize(); ++i)
            {
                unsigned char c = static_cast<unsigned char>(Cfracture[i]);
                float d = static_cast<float>(c);
                _data.push_back(d);
            }

            //for debug
            qDebug() << "the number of data C which is collected is: " << _data.size();
            for(QVector<double>::const_iterator it = _data.begin();it != _data.end();++it)
            {
                std::cout << *it << " " << std::endl;
            }
            std::cout << std::endl;

            emit DataToDraw(current_curve_name,_data);
            _data.erase(_data.begin(),_data.end());
        }
        if(current_curve_name == tr("位移量"))
        {
            QVector<char> displacement = _data_cache.PopDataFromDisplacement();
            for(unsigned int i = 0; i < _data_cache.DisplacementSize(); ++i)
            {
                unsigned char c = static_cast<unsigned char>(displacement[i]);
                float d = static_cast<float>(c);
                _data.push_back(d);
            }

            //for debug
            qDebug() << "the number of data  displacement which is collected is: " << _data.size();
            for(QVector<double>::const_iterator it = _data.begin();it != _data.end();++it)
            {
                std::cout << *it << " " << std::endl;
            }
            std::cout << std::endl;

            emit DataToDraw(current_curve_name,_data);
            _data.erase(_data.begin(),_data.end());
        }
        if(current_curve_name == tr("分合闸线圈电流"))
        {
            QVector<char> coil_current = _data_cache.PopDataFromCoilCurrent();
            for(unsigned int i = 0; i < _data_cache.CoilCurrentSize(); ++i)
            {
                unsigned char c = static_cast<unsigned char>(coil_current[i]);
                float d = static_cast<float>(c);
                _data.push_back(d);
            }

            //for debug
            qDebug() << "the number of data coil current which is collected is: " << _data.size();
            for(QVector<double>::const_iterator it = _data.begin();it != _data.end();++it)
            {
                std::cout << *it << " " << std::endl;
            }
            std::cout << std::endl;

            emit DataToDraw(current_curve_name,_data);
            _data.erase(_data.begin(),_data.end());
        }
    }
}

uint16_t CustomProtocol::CheckSum()
{
    uint16_t sum = 0;
    for(int i = 0; i < _frame_current_num + 5; ++i) //5为报文头长度
    {
        sum += (unsigned char)_frame_data_from_server[i];
    }
    return sum;
}

uint16_t CustomProtocol::CaculateCheckSumToServer(unsigned int begin,unsigned int end)
{
    uint16_t sum = 0;
    for(unsigned int i = begin; i < end; ++i)
    {
        sum += (unsigned char)_frame_data_to_server[i];
    }
    return sum;
}

void CustomProtocol::Stop()
{
    _lock.lock();
    _running = false;
    _lock.unlock();
}

bool CustomProtocol::IsRunning() const
{
    _lock.lock();
    bool r = _running;
    _lock.unlock();
    return r;
}

void CustomProtocol::OpenSerialPort(QString serial_name, unsigned int baud_rate)
{
    _serial_name = serial_name;

    switch (baud_rate)
    {
    case 115200:
        _baud_rate = B115200;
        break;
    case 9600:
        _baud_rate = B9600;
        break;
    default:
        LOG(ERROR) << "baud rate is wrong,please choose right baud rate";
        break;
    }

    emit DataProcessStart();
}
