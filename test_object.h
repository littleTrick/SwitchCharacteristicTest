#ifndef TEST_OBJECT_H
#define TEST_OBJECT_H

#include <QDialog>
#include <map>
#include "inttypes.h"

namespace Ui {
class TestObject;
}

class TestObject : public QDialog
{
    Q_OBJECT
    
public:
    explicit TestObject(QWidget *parent = 0);
    ~TestObject();

    void InitCollectDataPara();
    void InitTestObject(QString sampling_freq,QString sampling_cycle,QString fracture_before,QString fracture_after);
    void SetCollectDataPara();
    unsigned int GetRangeUpper();

signals:
    void SendCollectDataPara(QStringList ,uint8_t,uint8_t,uint32_t,uint32_t);

private slots:
    void onSetCollectDataPara();

private:
    Ui::TestObject *ui;
    std::map<QString,int> _data_list;
    QStringList _selected_data_name;
    std::vector<unsigned int> _selected_data_item;
    uint32_t _range_lower;
    uint32_t _range_upper;
    uint8_t _min_selected_item;
    uint8_t _max_selected_item;

};

#endif // TEST_OBJECT_H
