#ifndef _WINDOWS_STARTUP_H_
#define _WINDOWS_STARTUP_H_

#include "../../iSingleton.h"

#include <WinSock2.h>
#include <Windows.h>

namespace Common {
namespace network {
namespace windows {

class WindowsStartup: public Singleton<WindowsStartup>
{
MAKE_SINGLETON(WindowsStartup);
public:
	inline bool IsInitialized();
	inline bool HasError();
	bool Initialize();
private:
	WindowsStartup();
	~WindowsStartup();
private:
	bool m_isInitialized;
	bool m_hasError;
};

bool WindowsStartup::IsInitialized()
{
	return m_isInitialized;
}

bool WindowsStartup::HasError()
{
	return m_hasError;
}

} //namespace windows
} //namespace network
} //namespace Common

#endif //_WINDOWS_STARTUP_H_