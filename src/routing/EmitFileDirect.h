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
#include "qamqp/amqp_queue.h"

#include <assert.h>

namespace QAMQP
{

namespace samples
{

class EmitFileDirect : public QObject , public QRunnable
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
	verbose_ = 0;
	// Create AMQP client
	client_ = new QAMQP::Client(this);
	client_->open(QUrl(address));
	//client_->setAutoReconnect(true);

	// Create the "direct_files" exchange
	exchange_ =  client_->createExchange("direct_files");
	exchange_->declare("direct");

        // Create the "files" queue
        // This isn't mandatory but if it doesn't exist, the messages are lost
        client_
            ->createQueue("files", exchange_->channelNumber())
            ->declare();

	BuildFileList();

	if (verbose_) 
	{
	    qDebug() << "Number of files found: " << files_.size();
	    for (int i=0; i<files_.size(); i++)
		qDebug() << "      File[" << i << "]=" << files_[i];
	}
    }

    void BuildFileList()
    {
 	// set list of files in the directory using file filters, if any
	QStringList files = dir_.entryList(filters_);

	files_.clear();
	QStringList::const_iterator it;
        for (it = files.constBegin(); it != files.constEnd(); ++it)
	{
	    if ((*it) == "." ) continue;
	    if ((*it) == "..") continue;

	    files_ += (*it);
	}
    }

    void run()
    {
	QTimer* timer = new QTimer(this);
	timer->setInterval(100);
	connect(timer, SIGNAL(timeout()), SLOT(EmitFileMessage()));
	timer->start();
    }

protected slots:

    void EmitFileMessage()
    {
        // Choose random key
        QString key(list_.at(qrand() % list_.size()));

	if (files_.isEmpty()) {
            qDebug() << "EmitFileDirect::EmitFileMessage() -- end-of-files in directory ";
	    BuildFileList();
	}

	assert (! files_.isEmpty());

	// get a file from the top of the stack
	QString file=files_[0];
	files_.pop_front();

	// get the contents of the file
	QFile fd(dir_.filePath(file));
	if (! fd.open(QIODevice::ReadOnly)) 
	{
	    qDebug() << "EmitFileDirect::EmitFileMessage() Error, could not open file: " << dir_.absolutePath() << "/" << file;
	    return;
	}
	QByteArray data = fd.readAll();
	fd.close();

        // Create Message
	QAMQP::Exchange::MessageProperties m_prop;  // use default message properties
	QString mime_type("image/jpeg");            // assume all data files are binary
        QByteArray ba_message;                      // build the message byte array

	ba_message.append(data);

	qDebug() << "EmitFileDirect::EmitFileMessage() Sending file: " << file;

	QVariantHash headers;
	headers["fname"] = file;

	if (verbose_) 
	{
	    QHashIterator<QString, QVariant> i(headers);
	    while (i.hasNext()) {
		i.next();
		qDebug() << i.key() << ": " << i.value();
	    }
	}

        exchange_->publish(ba_message, key, headers, mime_type, m_prop);
    }

private:
    QStringList      list_;
    QAMQP::Exchange* exchange_;
    QAMQP::Client*   client_;
    QDir             dir_;
    QStringList      filters_;
    QStringList      files_;
    QAMQP::Queue*    queue_;
    int              verbose_;
};

}

}

#endif // EMITFILEDIRECT_H
