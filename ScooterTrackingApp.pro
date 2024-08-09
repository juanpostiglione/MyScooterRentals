QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    accountmanagement.cpp \
    adddeletescooter.cpp \
    changedata.cpp \
    deleteuser.cpp \
    employee_manager.cpp \
    main.cpp \
    mainwindow.cpp \
    myscooters.cpp \
    promoteaccount.cpp \
    rentscooter.cpp \
    scooter_management.cpp \
    scooterrequests.cpp \
    settingswindow.cpp

HEADERS += \
    accountmanagement.h \
    adddeletescooter.h \
    changedata.h \
    deleteuser.h \
    employee_manager.h \
    mainwindow.h \
    myscooters.h \
    promoteaccount.h \
    rentscooter.h \
    scooter_management.h \
    scooterrequests.h \
    settingswindow.h

FORMS += \
    accountmanagement.ui \
    adddeletescooter.ui \
    changedata.ui \
    deleteuser.ui \
    employee_manager.ui \
    mainwindow.ui \
    myscooters.ui \
    promoteaccount.ui \
    rentscooter.ui \
    scooter_management.ui \
    scooterrequests.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
