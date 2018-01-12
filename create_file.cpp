#include "create_file.h"
#include "ui_create_file.h"

CreateFile::CreateFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateFile)
{
    ui->setupUi(this);
}

CreateFile::~CreateFile()
{
    delete ui;
}

void CreateFile::SetDefaultPath(QString default_path)
{
    ui->default_path->setText(default_path);
}
