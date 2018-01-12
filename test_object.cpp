#include "test_object.h"
#include "ui_test_object.h"
#include "algorithm"
#include <QDebug>

using namespace std;

TestObject::TestObject(QWidget *parent) :
    QDialog(parent),
    _range_upper(0),
    ui(new Ui::TestObject)
{
    ui->setupUi(this);

    connect(this,SIGNAL(finished(int)),this,SLOT(onSetCollectDataPara()));
    InitCollectDataPara();
}

TestObject::~TestObject()
{
    delete ui;
}

void TestObject::InitTestObject(QString sampling_freq, QString sampling_cycle, QString fracture_before, QString fracture_after)
{
    ui->sampling_frequence->setText(sampling_freq);
    ui->sampling_cycle->setText(sampling_cycle);
    ui->fracture_before->setText(fracture_before);
    ui->fracture_after->setText(fracture_after);
}

void TestObject::InitCollectDataPara()
{
    _data_list.insert(make_pair(tr("A相断口"),1));
    _data_list.insert(make_pair(tr("B相断口"),2));
    _data_list.insert(make_pair(tr("C相断口"),3));
    _data_list.insert(make_pair(tr("位移量"),4));
    _data_list.insert(make_pair(tr("分合闸线圈电流"),5));
}

void TestObject::SetCollectDataPara()
{
    if(ui->A_fracture->isChecked())
    {
        _selected_data_item.push_back(1); //1表示A相断口
        _selected_data_name << tr("A相断口");
    }
    if(ui->B_fracture->isChecked())
    {
        _selected_data_item.push_back(2);//2表示B相断口
        _selected_data_name << tr("B相断口");
    }
    if(ui->C_fracture->isChecked())
    {
        _selected_data_item.push_back(3);//3表示C相断口
        _selected_data_name << tr("C相断口");

    }
    if(ui->displacement->isCheckable())
    {
        _selected_data_item.push_back(4);//4表示位移量
        _selected_data_name << tr("位移量");
    }
    if(ui->coil_current->isChecked())
    {
        _selected_data_item.push_back(5);//5表示分合闸线圈电流
        _selected_data_name << tr("分合闸线圈电流");
    }

    _min_selected_item = *min_element(_selected_data_item.begin(),_selected_data_item.end());
    _max_selected_item = *max_element(_selected_data_item.begin(),_selected_data_item.end());

    unsigned int freq = ui->sampling_frequence->text().toUInt();
    unsigned int cycle = ui->sampling_cycle->text().toUInt();
    _range_upper = freq * cycle;
}

void TestObject::onSetCollectDataPara()
{
    SetCollectDataPara();
    _range_lower = 0;
    emit SendCollectDataPara(_selected_data_name,_min_selected_item,_max_selected_item,_range_lower,_range_upper);
}

unsigned int TestObject::GetRangeUpper()
{
    return _range_upper;
}
