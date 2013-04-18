include(qamqp_general.pri)

TEMPLATE = app
TARGET = qamqp

DESTDIR = $${INSTALL_DIR}/bin

INCLUDEPATH += . $${INSTALL_DIR}/include/
DEPENDPATH  += . $${INSTALL_DIR}/include/

QMAKE_LIBDIR += $${INSTALL_DIR}/lib

LIBS += -lQAMQP

SOURCES += src/main.cpp

HEADERS += src/QamqpApp.h
