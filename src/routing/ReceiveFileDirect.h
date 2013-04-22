#ifndef RECEIVEFILEDIRECT_H
#define RECEIVEFILEDIRECT_H

#include <QObject>
#include <QRunnable>
#include <QDebug>
#include <QStringList>
#include <QThread>
#include <QTime>
#include <QDir>

#include "qamqp/amqp.h"
#include "qamqp/amqp_queue.h"


namespace QAMQP
{

namespace samples
{

class ReceiveFileDirect : public QObject, public QRunnable
{
    Q_OBJECT

    typedef QObject super;

public:
    explicit ReceiveFileDirect(const QString& address, const QString& list, const QString& dir, QObject* parent)
        : super(parent)
        , list_(list)
        , dir_(dir)
    {
        // Create AMQP client
        QAMQP::Client* client = new QAMQP::Client(this);
        client->open(QUrl(address));

        // Create an exclusive queue
        queue_ = client->createQueue();
        queue_->declare("", Queue::Exclusive);

        connect(queue_, SIGNAL(declared()), this, SLOT(declared()));
        connect(queue_, SIGNAL(messageReceived(QAMQP::Queue*)), this, SLOT(newMessage(QAMQP::Queue*)));
    }

    void run()
    {
    }

protected slots:
    void declared()
    {
        // Loop on the list to bind with the keys
        QStringList split(list_.split(',', QString::SkipEmptyParts));
        for(int i = 0; i < split.size(); ++i)
            queue_->bind("direct_files", split.at(i));

        // Start consuming
        queue_->consume(QAMQP::Queue::coNoAck);
    }

    void newMessage(QAMQP::Queue* queue)
    {
	// file contents starts immediately after the data_tag
	const QString data_tag("data=");
	if (queue != queue_) {
	    qDebug() << "ReceiveFileDirect::newMessage() " << "Ignoring message received from a different queue";
	}

        // Retrieve message
        QAMQP::MessagePtr message = queue_->getMessage();
        int msg_length    = message->payload.length();
	int file_start    = message->payload.indexOf("[");
	int file_end      = message->payload.indexOf("]");
	QString file_name = message->payload.mid(file_start+1, file_end-file_start-1);
	int data_index    = message->payload.indexOf(data_tag);

	QByteArray data = message->payload.right(msg_length-(data_index+data_tag.size()));

	QFile fd(dir_.filePath(file_name));
	if (! fd.open(QIODevice::WriteOnly)) {
	    qDebug() << "ReceiveFileDirect::newMessage() Error, could not open file: " << dir_.absolutePath() << "/" << file_name;
	    return;
	}
        fd.write(data);
	fd.close();
	qDebug() << "ReceiveFileDirect::newMessage() Received file: " << file_name;
    }

private:
    QString list_;
    QAMQP::Queue* queue_;
    QDir    dir_;
};

}

}

#endif // RECEIVEFILEDIRECT_H
