#include "WindowsStartup.h"

#include <WinSock2.h>
#include <Windows.h>

namespace Common {
namespace network {
namespace windows {

WindowsStartup::WindowsStartup():
	m_hasError(false), m_isInitialized(false)
{
	
}

bool WindowsStartup::Initialize()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(result == 0)
	{
		m_hasError = false;
		m_isInitialized = true;
		return true;
	}
	else
	{
		m_hasError = true;
		m_isInitialized = false;
		return false;
	}
}

WindowsStartup::~WindowsStartup()
{
	WSACleanup();
}

} //namespace windows
} //namespace network
} //namespace Common