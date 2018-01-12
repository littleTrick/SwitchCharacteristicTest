#include "life_test.h"
#include "ui_life_test.h"

LifeTest::LifeTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LifeTest)
{
    ui->setupUi(this);
}

LifeTest::~LifeTest()
{
    delete ui;
}

void LifeTest::InitLifeTest(QString test_time, QString interval_time)
{
    ui->test_time->setText(test_time);
    ui->interval_time->setText(interval_time);
}
