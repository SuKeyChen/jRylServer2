//#include "Launcher.h"
//
//#include "../Common/Logger.h"
//
//#include <Poco/Net/SocketReactor.h>
//#include <Poco/NObserver.h>
//#include <Poco/Net/SocketNotification.h>
//
//Laucher::Laucher(const Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& socketReactor):
//TCPServerConnection(socket), m_socket(socket), m_socketReactor(socketReactor)
//{
//	m_socketReactor.addEventHandler(m_socket, Poco::NObserver<Laucher, Poco::Net::ReadableNotification>(*this, &Laucher::onReadable));
//	m_socketReactor.addEventHandler(m_socket, Poco::NObserver<Laucher, Poco::Net::ShutdownNotification>(*this, &Laucher::onShutdown));
//}
//
//void Laucher::run()
//{
//	
//}
//
//void Laucher::onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf)
//{
//	
//}
//
//void Laucher::onShutdown(const Poco::AutoPtr<Poco::Net::ShutdownNotification>& pNf)
//{
//	
//}