QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authorwindow.cpp \
    bookinformation.cpp \
    bookwindow.cpp \
    customerwindow.cpp \
    dbsingleton.cpp \
    directorwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    managerwindow.cpp \
    mytableview.cpp \
    orderitemswindow.cpp \
    orderwindow.cpp \
    publisherwindow.cpp \
    registrationwindow.cpp \
    storewindow.cpp

HEADERS += \
    authorwindow.h \
    bookinformation.h \
    bookwindow.h \
    customerwindow.h \
    dbsingleton.h \
    directorwindow.h \
    mainwindow.h \
    managerwindow.h \
    mytableview.h \
    orderitemswindow.h \
    orderwindow.h \
    publisherwindow.h \
    registrationwindow.h \
    storewindow.h

FORMS += \
    authorwindow.ui \
    bookinformation.ui \
    bookwindow.ui \
    customerwindow.ui \
    directorwindow.ui \
    mainwindow.ui \
    managerwindow.ui \
    orderitemswindow.ui \
    orderwindow.ui \
    publisherwindow.ui \
    registrationwindow.ui \
    storewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
