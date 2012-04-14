#ifndef _LAUNCHER_H_
#define _LAUNCHER_H_

#include <Poco/Net/TCPServerConnection.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/SocketNotification.h>

class Laucher: public Poco::Net::TCPServerConnection {
public:
	Laucher(const Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& socketReactor);
	void run();
	void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification>& pNf);
	void onShutdown(const Poco::AutoPtr<Poco::Net::ShutdownNotification>& pNf);
private:
	Poco::Net::SocketReactor& m_socketReactor;
	const Poco::Net::StreamSocket& m_socket;
};

#endif