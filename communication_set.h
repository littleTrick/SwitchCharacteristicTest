#ifndef COMMUNICATION_SET_H
#define COMMUNICATION_SET_H

#include <QDialog>

namespace Ui {
class CommunicationSet;
}

class CommunicationSet : public QDialog
{
    Q_OBJECT
    
public:
    explicit CommunicationSet(QWidget *parent = 0);
    ~CommunicationSet();

    void SetSerialPort(QString serial_port);

signals:
    void OpenSerialClicked( QString protocol,QString serial_port,unsigned int baud_rate);

private slots:
    void onOpenSerial();
    
private:
    Ui::CommunicationSet *ui;
};

#endif // COMMUNICATION_SET_H
