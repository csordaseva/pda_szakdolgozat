QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++1z

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pda.cpp

HEADERS += \
    mainwindow.h \
    pda.h \
    tree.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += "C:\Program Files\Graphviz\include\graphviz"

LIBS += -L"C:\Program Files\Graphviz\lib" -lgvc
LIBS += -L"C:\Program Files\Graphviz\lib" -lcgraph

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
