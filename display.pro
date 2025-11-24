#-------------------------------------------------
#
# Project created by QtCreator 2024-07-25T09:43:34
#
#-------------------------------------------------

QT       += core gui widgets network serialport charts printsupport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = display
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        qcustomplot.cpp\
        widget.cpp \
    tcp.cpp \
    serial.cpp \
    dac8760.cpp \
    mchart.cpp \
    form_measure.cpp \
    form_param.cpp \
    form_calibrate.cpp \
    material_sound_speed_manager.cpp \
    modbus_server.cpp \
    g_var.cpp \
    prasingRecvCmd.cpp \
    sendCmd.cpp \
    serial_communicate.cpp

HEADERS += \
         qcustomplot.h\
        widget.h \
    tcp.h \
    serial.h \
    dac8760.h \
    wiringPi.h \
    mchart.h \
    form_measure.h \
    form_param.h \
    form_calibrate.h \
    type_define.h \ 
    TimeUtils.h \
    param_define.h \
    type_define.h \
    material_sound_speed_manager.h \
    modbus_server.h \
    g_var.h \
    prasingRecvCmd.h \
    sendCmd.h \
    serial_communicate.h
    

   

FORMS += \
        widget.ui \
    form_measure.ui \
    form_param.ui \
    form_calibrate.ui
LIBS += -L/usr/local/lib -lwiringPi

INCLUDEPATH += \
    $$PWD/.. \         
    /usr/local/include \
    /usr/include 

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
