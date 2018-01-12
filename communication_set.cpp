#include "communication_set.h"
#include "ui_communication_set.h"

CommunicationSet::CommunicationSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommunicationSet)
{
    ui->setupUi(this);
    connect(ui->btn_open_serial,SIGNAL(clicked()),this,SLOT(onOpenSerial()));
}

CommunicationSet::~CommunicationSet()
{
    delete ui;
}

void CommunicationSet::SetSerialPort(QString serial_port)
{
    ui->serial_port->setText(serial_port);
}

void CommunicationSet::onOpenSerial()
{
    emit OpenSerialClicked(ui->protocol->currentText(),ui->serial_port->text(),ui->baud_rate->currentText().toUInt());
}
