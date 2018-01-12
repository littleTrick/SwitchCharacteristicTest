#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <QString>
#include <termios.h>

class SerialPort
{
public:
    SerialPort(const char *name) : name_(name), fd_(-1)
    {
    }
    SerialPort(const std::string &name) : name_(name), fd_(-1)
    {
    }
    SerialPort():fd_(-1)
    {

    }
    ~SerialPort();

    bool Open();
    void Close();

    // DEFAULT: 115200
    bool SetSpeed(unsigned int speed);
    // DEFAULT: false
    bool SetParity(bool parity);

    // Return bytes of read, -1 on error
    int Read(char *buff, int size);
    // Return 0 on success, otherwise -1
    int Write(const char *buff,int size);

    int fd() const{
        return fd_;
    }

    const std::string &name() const {
        return name_;
    }
    void SetSerialPortName(QString name)
    {
        name_ = name.toStdString();
    }

private:
    // disable copy and assignment
    SerialPort(const SerialPort &);
    void operator=(const SerialPort &);

    const static int kDefaultSpeed = B115200;

    std::string name_;
    int fd_;
};

#endif // SERIALPORT_H
