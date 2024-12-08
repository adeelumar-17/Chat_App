QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    hashutil.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    signup.cpp

HEADERS += \
    database.h \
    hashutil.h \
    login.h \
    mainwindow.h \
    signup.h

FORMS += \
    login.ui \
    mainwindow.ui \
    signup.ui

TRANSLATIONS += \
    CN_Proj_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/chat_bg.qrc

DISTFILES += \
    Resources/file.png \
    Resources/image.png \
    Resources/pdf.png
