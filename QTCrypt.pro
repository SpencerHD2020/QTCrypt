QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cryptfiledevice.cpp \
    encryption.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cryptfiledevice.h \
    encryption.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /usr/local/include
LIBS += /usr/local/opt/openssl@3/lib/libcrypto.a
LIBS += /usr/local/opt/openssl@3/lib/libssl.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
