#include <cassert>
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "serialport.h"

SerialPort::~SerialPort()
{
    Close();
}

bool SerialPort::Open()
{
    if (fd_ >= 0)
    {
        printf("%s alreay opened\n", name_.c_str());
        return false;
    }

    fd_ = open(name_.c_str(), O_RDWR|O_NOCTTY);
    if(fd_ < 0)
    {
        printf("Failed to open %s: %s\n", name_.c_str(), strerror(errno));
        return false;
    }

    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr(fd_, &tty) != 0)
    {
        printf("tcgetattr error(%d): %s\n", errno, strerror(errno));
        return false;
    }

    cfsetospeed (&tty, kDefaultSpeed);
    cfsetispeed (&tty, kDefaultSpeed);

    tty.c_cflag |= (CREAD | CLOCAL);  // turn on READ & ignore ctrl lines

    // no parity bit
    tty.c_cflag &= ~PARENB;
    // 1 stop bit (if enabled, 2 stop bits)
    tty.c_cflag &= ~CSTOPB;
    //disable hardware flow control
    tty.c_cflag &= ~CRTSCTS;
    // 8 data bits
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    // http://www.cmrr.umn.edu/~strupp/serial.html
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 30;  // timeout, in 0.1 seconds

    // 原始输入
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // 原始输出
    tty.c_oflag  &= ~OPOST;   /*Output*/

    tcflush(fd_, TCIFLUSH);
    if (tcsetattr (fd_, TCSANOW, &tty) != 0)
    {
        printf ("tcsetattr error: %s\n", strerror(errno));
        return false;
    }
    return true;
}

int SerialPort::Read(char *buff, int size)
{
    return read(fd_,buff,size);
}

int SerialPort::Write(const char *buff, int size)
{
    int nwrite = 0;
    while(nwrite < size)
    {
        int n = write(fd_,buff + nwrite,size - nwrite);
        if(n > 0)
        {
            nwrite += n;
        }
        else if( n == -1)
        {
            //add information to log.txt
        }
    }
    return nwrite;
    //return write(fd_,buff,size);
}

void SerialPort::Close()
{
    if (fd_ >= 0) {
#ifdef DEBUG
        printf("closing %s\n", name_.c_str());
#endif
        close(fd_);
    }
}

bool SerialPort::SetSpeed(unsigned int speed)
{
    struct termios tty;
    if (tcgetattr (fd_, &tty) != 0)
    {
#ifdef DEBUG
        printf("error %d from tcgetattr", errno);
#endif
        return false;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    if (tcsetattr (fd_, TCSANOW, &tty) != 0)
    {
#ifdef DEBUG
        printf ("error %d from tcsetattr", errno);
#endif
        return false;
    }
    return true;
}

bool SerialPort::SetParity(bool parity)
{
    struct termios tty;
    if (tcgetattr (fd_, &tty) != 0)
    {
#ifdef DEBUG
        printf("error %d from tcgetattr", errno);
#endif
        return false;
    }

    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= (parity ? 1 : 0);

    if (tcsetattr (fd_, TCSANOW, &tty) != 0)
    {
#ifdef DEBUG
        printf ("error %d from tcsetattr", errno);
#endif
        return false;
    }
    return true;
}

