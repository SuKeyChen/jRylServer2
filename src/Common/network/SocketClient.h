#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include "windows/WindowsSocketClient.h"

namespace Common {
namespace network {
	typedef windows::WindowsSocketClient SocketClient;
}
}

#endif