#-------------------------------------------------
#
# Project created by QtCreator 2017-12-28T08:51:30
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SwitchCharacteristicTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logging.cpp \
    timestamp.cc \
    login.cpp \
    controller.cpp \
    about.cpp \
    create_file.cpp \
    communication_set.cpp \
    sensor_check.cpp \
    test_object.cpp \
    life_test.cpp \
    serialport.cpp \
    process_data_thread.cpp \
    custom_protocol.cpp \
    data_cache.cpp

HEADERS  += mainwindow.h \
    logging.h \
    timestamp.h \
    login.h \
    controller.h \
    about.h \
    create_file.h \
    communication_set.h \
    sensor_check.h \
    test_object.h \
    life_test.h \
    serialport.h \
    process_data_thread.h \
    custom_protocol.h \
    data_cache.h

FORMS    += mainwindow.ui \
    login.ui \
    about.ui \
    create_file.ui \
    communication_set.ui \
    sensor_check.ui \
    test_object.ui \
    life_test.ui

INCLUDEPATH += /usr/local/qwt-6.1.4-svn/include
LIBS += -L"/usr/local/qwt-6.1.4-svn/lib/" -lqwt

OTHER_FILES +=

RESOURCES += \
    background.qrc
