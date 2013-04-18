include(qamqp_general.pri)

DESTDIR = $${INSTALL_DIR}/lib

INCLUDEPATH    += $${INSTALL_DIR}/include/
INCLUDEPATH    += ../.
INCLUDEPATH    += src

headers.path = $${INSTALL_DIR}/include/qamqp/
headers.files += src/qamqp/*.h

INSTALLS += headers

QT += sql
QT += network

CONFIG += debug
CONFIG -= release

include(qamqp_lib.pri)
