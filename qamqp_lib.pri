# Source/Headers required to build a qamqp library

HEADERS += src/qamqp/amqp.h
HEADERS += src/qamqp/amqp_authenticator.h
HEADERS += src/qamqp/amqp_channel.h
HEADERS += src/qamqp/amqp_channel_p.h
HEADERS += src/qamqp/amqp_connection.h
HEADERS += src/qamqp/amqp_connection_p.h
HEADERS += src/qamqp/amqp_exchange.h
HEADERS += src/qamqp/amqp_exchange_p.h
HEADERS += src/qamqp/amqp_frame.h
HEADERS += src/qamqp/amqp_message.h
HEADERS += src/qamqp/amqp_network.h
HEADERS += src/qamqp/amqp_p.h
HEADERS += src/qamqp/amqp_queue.h
HEADERS += src/qamqp/amqp_queue_p.h
HEADERS += src/qamqp/amqp_global.h 

SOURCES += src/qamqp/amqp.cpp
SOURCES += src/qamqp/amqp_authenticator.cpp
SOURCES += src/qamqp/amqp_channel.cpp
SOURCES += src/qamqp/amqp_connection.cpp
SOURCES += src/qamqp/amqp_exchange.cpp
SOURCES += src/qamqp/amqp_frame.cpp
SOURCES += src/qamqp/amqp_network.cpp
SOURCES += src/qamqp/amqp_queue.cpp 

# headers needed to include moc objects in library
HEADERS += src/routing/ReceiveLogDirect.h
HEADERS += src/routing/EmitLogDirect.h
HEADERS += src/pubsub/EmitLog.h
HEADERS += src/pubsub/ReceiveLog.h
HEADERS += src/workqueues/NewTask.h
HEADERS += src/workqueues/Worker.h
HEADERS += src/sendreceive/Send.h
HEADERS += src/sendreceive/Receive.h
HEADERS += src/routing/ReceiveFileDirect.h
HEADERS += src/routing/EmitFileDirect.h
