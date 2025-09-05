QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets widgets charts printsupport
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commande.cpp \
    connection.cpp \
    livreur.cpp \
    main.cpp \
    mainwindow.cpp \
    statcommande.cpp \
    statlivreur.cpp \
    statswindow.cpp

HEADERS += \
    commande.h \
    connection.h \
    livreur.h \
    mainwindow.h \
    statcommande.h \
    statlivreur.h \
    statswindow.h

FORMS += \
    mainwindow.ui \
    statcommande.ui \
    statlivreur.ui \
    statswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    images.qrc \
    resources.qrc
