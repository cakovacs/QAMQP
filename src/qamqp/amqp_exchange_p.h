#include "amqp_channel_p.h"
#define METHOD_ID_ENUM(name, id) name = id, name ## Ok

namespace QAMQP
{
	using namespace QAMQP::Frame;
	class ExchangePrivate: public ChannelPrivate
	{
		Q_DECLARE_PUBLIC(QAMQP::Exchange)
	public:

		enum MethodId
		{
			METHOD_ID_ENUM(miDeclare, 10),
			METHOD_ID_ENUM(miDelete, 20)
		};

		ExchangePrivate();
		~ExchangePrivate();

		void declare();
		void remove(bool ifUnused = true, bool noWait = true);

		void declareOk(const QAMQP::Frame::Method & frame);
		void deleteOk(const QAMQP::Frame::Method & frame);

		void publish(const QByteArray & message, const QString & key, const QString &mimeType = QString::fromLatin1("text/plain"));

		QString type;
		Exchange::ExchangeOptions options;
		TableField arguments;
		
		void _q_method(const QAMQP::Frame::Method & frame);
		
		bool deleyedDeclare;
		bool declared;

	};	
}