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
	qDebug() << "---------------->>>>> input filter = " << filter;
	// Create AMQP client
	QAMQP::Client* client = new QAMQP::Client(this);
	client->open(QUrl(address));

	// Create the "direct_logs" fanout exchange
	exchange_ =  client->createExchange("direct_logs");
	exchange_->declare("direct");

	// set list of files in the directory using file filters, if any
	files_ = dir_.entryList(filters_);
	qDebug() << "----------- Directory: " << dir_.absolutePath();
	qDebug() << "----------- Filters: " << filters_;
	qDebug() << "----------- Number of files found: " << files_.size();
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
	QString file=files_[0];
	files_.pop_front();

        // Create Message
        QString message(QString("[%1: %2] %3")
          .arg(++counter)
          .arg(file)
          .arg(key)
          );
        qDebug() << "EmitFileDirect::EmitFileMessage " << message;

        // Publish
        exchange_->publish(message, key);
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
