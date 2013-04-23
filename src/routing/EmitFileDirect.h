#ifndef EMITFILEDIRECT_H
#define EMITFILEDIRECT_H

#include <QObject>
#include <QRunnable>
#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QDateTime>
#include <QDir>

#include "qamqp/amqp.h"
#include "qamqp/amqp_exchange.h"

namespace QAMQP
{

namespace samples
{

class EmitFileDirect : public QObject, public QRunnable
{
    Q_OBJECT

    typedef QObject super;

public:
    explicit EmitFileDirect(const QString& address, const QString& list, const QString& dir, const QString& filter, QObject *parent)
        : super(parent)
        , list_(list.split(',', QString::SkipEmptyParts))
	, dir_(dir)
	, filters_(filter.split(',', QString::SkipEmptyParts))
    {
	// Create AMQP client
	QAMQP::Client* client = new QAMQP::Client(this);
	client->open(QUrl(address));

	// Create the "direct_files" fanout exchange
	exchange_ =  client->createExchange("direct_files");
	exchange_->declare("direct");

	// set list of files in the directory using file filters, if any
	files_ = dir_.entryList(filters_);
	qDebug() << "Number of files found: " << files_.size();
	for (int i=0; i<files_.size(); i++)
	    qDebug() << "      File[" << i << "]=" << files_[i];
    }

    void run()
    {
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, SIGNAL(timeout()), SLOT(EmitFileMessage()));
        timer->start();
    }

protected slots:
    void EmitFileMessage()
    {
        static quint64 counter = 0;

        // Choose random key
        QString key(list_.at(qrand() % list_.size()));

	if (files_.isEmpty()) {
            qDebug() << "EmitFileDirect::EmitFileMessage() -- end-of-files in directory ";
	    return;
	}

	// get a file from the top of the stack
	QString file=files_[0];
	files_.pop_front();


	// get the contents of the file
	QFile fd(dir_.filePath(file));
	if (! fd.open(QIODevice::ReadOnly)) {
	    qDebug() << "EmitFileDirect::EmitFileMessage() Error, could not open file: " << dir_.absolutePath() << "/" << file;
	    return;
	}
	QByteArray data = fd.readAll();
	fd.close();

        // Create Message
	QAMQP::Exchange::MessageProperties m_prop;  // use default message properties
	QString mime_type("geopro/octet-stream");   // assume all data files are binary
        QByteArray ba_message;                      // build the message byte array

        QString message_header(QString("%1: [%2] %3 data=")
          .arg(++counter)
          .arg(file)
          .arg(key)
          );
	
        ba_message.append(message_header);
	ba_message.append(data);

	qDebug() << "EmitFileDirect::EmitFileMessage() Sending file: " << file;
        //exchange_->publish(ba_message, key, mime_type, m_prop);
	QVariantHash headers;
	headers["fname"] = file;
	headers["A"] = "AAAAAAAAAAA";
	headers["B"] = "BBBBB";
	headers["C"] = "CCCCCCCCCCCCCCCCC";


	//m_prop["ddddddddddddd"] = "ddddddddddd";

	qDebug() << "EmitFileDirect::EmitFileMessage() headers=" << headers;
	//qDebug() << "EmitFileDirect::EmitFileMessage() property=" << m_prop;

	QHashIterator<QString, QVariant> i(headers);
	while (i.hasNext()) {
	    i.next();
	    qDebug() << i.key() << ": " << i.value();
	}

        exchange_->publish(ba_message, key, headers, mime_type, m_prop);
    }

private:
    QStringList list_;
    QAMQP::Exchange* exchange_;
    QDir        dir_;
    QStringList filters_;
    QStringList files_;
};

}

}

#endif // EMITFILEDIRECT_H
