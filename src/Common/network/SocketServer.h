#ifndef _SOCKET_SERVER_H_
#define _SOCKET_SERVER_H_

#include "windows/WindowsSocketServer.h"

namespace Common {
namespace network {
	typedef windows::WindowsSocketServer SocketServer;
}
}

#endif