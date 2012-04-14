//#include "LauncherConnectionFactory.h"
//
//#include <Poco/Net/TCPServerConnectionFactory.h>
//#include <Poco/Thread.h>
//#include "Launcher.h"
//
//Poco::Net::TCPServerConnection* LauncherConnectionFactory::createConnection(const Poco::Net::StreamSocket& socket) 
//{
//	return new Laucher(socket, m_socketReactor);
//}
//
//LauncherConnectionFactory::LauncherConnectionFactory()
//{
//	m_threadSocketReactor.start(m_socketReactor);
//}
