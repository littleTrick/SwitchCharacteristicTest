#ifndef LIFE_TEST_H
#define LIFE_TEST_H

#include <QDialog>

namespace Ui {
class LifeTest;
}

class LifeTest : public QDialog
{
    Q_OBJECT
    
public:
    explicit LifeTest(QWidget *parent = 0);
    ~LifeTest();

    void InitLifeTest(QString test_time, QString interval_time);
    
private:
    Ui::LifeTest *ui;
};

#endif // LIFE_TEST_H
