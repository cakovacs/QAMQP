BUILD_TEMP_DIR  = .obj
INSTALL_DIR     = install
TEMPLATE        = lib
TARGET          = QAMQP
OBJECTS_DIR     = $${BUILD_TEMP_DIR}/QAMQP/
UI_SOURCES_DIR  = $${INSTALL_DIR}/include/QAMQP/ui/
MOC_DIR         = $${BUILD_TEMP_DIR}/QAMQP/moc/
DESTDIR         = $${INSTALL_DIR}/lib/

INCLUDEPATH    += $${INSTALL_DIR}/include/
INCLUDEPATH    += ../.

headers.path = $${INSTALL_DIR}/include/QAMQP/
headers.files += src/qamqp/*.h

INSTALLS += headers

QT += sql
QT += network

CONFIG += debug
CONFIG -= release

include(qamqp_lib.pri)
