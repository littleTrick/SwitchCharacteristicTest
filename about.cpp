#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    connect(ui->btn_close,SIGNAL(clicked()),this,SLOT(onBtnCloseClicked()));
}

About::~About()
{
    delete ui;
}

void About::onBtnCloseClicked()
{
    this->close();
}
