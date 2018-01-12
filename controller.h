#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "login.h"
#include "mainwindow.h"
#include "process_data_thread.h"

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();
    ~Controller();
    void Start();
    void Configure();

private slots:
    void OnMainWindowClosed();
    void ProcessStart();

private:
    Login _login;
    MainWindow _main_window;
    ProcessDataThread _process_data_thread;
};

#endif // CONTROLLER_H
