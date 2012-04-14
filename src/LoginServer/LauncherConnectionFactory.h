#ifndef _LAUNCHER_CONNECTION_FACTORY_H_
#define _LAUNCHER_CONNECTION_FACTORY_H_

#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Thread.h>

class LauncherConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
	LauncherConnectionFactory();
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket);
	Poco::Net::SocketReactor m_socketReactor;
	Poco::Thread m_threadSocketReactor;
};

#endif //_LAUNCHER_CONNECTION_FACTORY_H_