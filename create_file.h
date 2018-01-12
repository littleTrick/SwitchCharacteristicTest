#ifndef CREATE_FILE_H
#define CREATE_FILE_H

#include <QDialog>

namespace Ui {
class CreateFile;
}

class CreateFile : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateFile(QWidget *parent = 0);
    ~CreateFile();

    void SetDefaultPath(QString default_path);
    
private:
    Ui::CreateFile *ui;
};

#endif // CREATE_FILE_H
