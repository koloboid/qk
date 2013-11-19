#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T18:48:34
#
#-------------------------------------------------

QT       -= gui
QT       += network

TARGET = qk.core
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += ../../
QMAKE_CXXFLAGS += -std=c++0x -Wall

SOURCES += \
    timespan.cpp \
    loglevel.cpp \
    logitem.cpp \
    loggertextio.cpp \
    logger.cpp \
    log.glibc.cpp \
    log.cpp \
    lock.cpp \
    error.cpp \
    enumitem.cpp \
    enum.cpp \
    consoledevice.cpp \
    config.cpp

HEADERS += \
    valref.hpp \
    timespan.hpp \
    loglevel.hpp \
    logitem.hpp \
    loggertextio.hpp \
    logger.hpp \
    log.hpp \
    lock.hpp \
    error.hpp \
    enumitem.hpp \
    enum.hpp \
    core.export.hpp \
    consoledevice.hpp \
    config.hpp \
    blockqueue.hpp \
    blob.hpp

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
