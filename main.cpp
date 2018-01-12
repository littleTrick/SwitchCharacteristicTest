#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include "login.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<double> >("QVector<double>");

    //qt中文乱码问题
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);

    Controller controller;
    controller.Start();

    return a.exec();
}
