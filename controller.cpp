#include "controller.h"
#include <QtXml>

Controller::Controller()
{
    connect(&_main_window,SIGNAL(closed()),this,SLOT(OnMainWindowClosed()));
    connect(&_main_window,SIGNAL(OpenSerialClicked(QString,QString,unsigned int)),
            &_process_data_thread,SIGNAL(OpenSerialClicked(QString,QString,unsigned int)));
    connect(&_main_window,SIGNAL(SendCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t)),
            &_process_data_thread,SIGNAL(SendCollectDataPara(QStringList,uint8_t,uint8_t,uint32_t,uint32_t)));
    connect(&_process_data_thread,SIGNAL(DataToDraw(const QString,const QVector<double>)),
            &_main_window,SLOT(DataToDraw(const QString,const QVector<double>)));
    connect(&_process_data_thread,SIGNAL(DataProcessStart()),
            this,SLOT(ProcessStart()));

}

Controller::~Controller()
{
    _process_data_thread.wait();
}

void Controller::Start()
{
    Configure();

    if(_login.exec() != QDialog::Accepted)
    {
        return;
    }
    else
    {
        _main_window.show();
    }
}

void Controller::Configure()
{
    QDomDocument document("mydocument");

    //Load the file
    QFile file(":/new/configure/configure.xml");//宏
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Failed to open file";
        return;
    }
    else
    {
        if(!document.setContent(&file))
        {
            qDebug() << "Failed to load document";
            file.close();
            return;
        }
        file.close();
    }

    QDomElement root = document.documentElement(); //get root node
    //qDebug() << root.nodeName();
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {
        if(node.isElement())
        {
            QDomElement e = node.toElement();
            QString tag_name = e.tagName();
            //qDebug() << tag_name;
            QDomNodeList list = e.childNodes();
            if(tag_name == "login")
            {
                QString user_name,password;
                for(int i = 0; i < list.count(); ++i)
                {
                    QDomNode n = list.at(i);
                    if(n.isElement())
                    {
                        if(n.nodeName() == "user_name")
                        {
                            user_name = n.toElement().text();
                        }
                        else if(n.nodeName() == "password")
                        {
                           password = n.toElement().text();
                        }
                    }
                }
                _login.InitLoginInfo(user_name,password);
            }
            else if(tag_name == "folder_path")
            {
                QString default_path;
                if(list.at(0).isElement())
                {
                    default_path = list.at(0).toElement().text();
                }
                _main_window.InitCreateFile(default_path);
            }
            else if(tag_name == "serial_attribute")
            {
                QString serial_port;
                if(list.at(0).isElement())
                {
                    serial_port = list.at(0).toElement().text();
                }
                _main_window.InitSerialAttribute(serial_port);
            }
            else if(tag_name == "life_test")
            {
                QString test_time,interval_time;
                for(int i = 0; i < list.count(); ++i)
                {
                    QDomNode n = list.at(i);
                    if(n.isElement())
                    {
                        if(n.nodeName() == "test_time")
                        {
                            test_time = n.toElement().text();
                        }
                        else if(n.nodeName() == "interval_time")
                        {
                           interval_time = n.toElement().text();
                        }
                    }
                }
                _main_window.InitLifeTest(test_time,interval_time);
            }
            else if(tag_name == "sensor_check")
            {
                QString current_coefficient,voltage_coefficient,acceleration_coefficient;
                for(int i = 0; i < list.count(); ++i)
                {
                    QDomNode n = list.at(i);
                    if(n.isElement())
                    {
                        if(n.nodeName() == "current_sensor_coefficient")
                        {
                            current_coefficient = n.toElement().text();
                        }
                        else if(n.nodeName() == "voltage_sensor_coefficient")
                        {
                            voltage_coefficient = n.toElement().text();
                        }
                        else if(n.nodeName() == "acceleration_sensor_coefficient")
                        {
                            acceleration_coefficient = n.toElement().text();
                        }
                    }
                }
                _main_window.InitSensorCheck(current_coefficient,voltage_coefficient,acceleration_coefficient);
            }
            else if(tag_name == "data_collection")
            {
                QString sampling_freq,sampling_cycle,fracture_before,fracture_after;
                for(int i = 0; i < list.count(); ++i)
                {
                    QDomNode n = list.at(i);
                    if(n.isElement())
                    {
                        if(n.nodeName() == "sampling_frequency")
                        {
                            sampling_freq = n.toElement().text();
                        }
                        else if(n.nodeName() == "sampling_cycle")
                        {
                            sampling_cycle = n.toElement().text();
                        }
                        else if(n.nodeName() == "fracture_before")
                        {
                            fracture_before = n.toElement().text();
                        }
                        else if(n.nodeName() == "fracture_after")
                        {
                            fracture_after = n.toElement().text();
                        }
                    }
                }
                _main_window.InitTestObject(sampling_freq,sampling_cycle,fracture_before,fracture_after);
            }

        }
        node = node.nextSibling();
    }
}

void Controller::ProcessStart()
{
    _process_data_thread.start();
}

void Controller::OnMainWindowClosed()
{
    qDebug() << "main window closed";
    _process_data_thread.Stop();
}
