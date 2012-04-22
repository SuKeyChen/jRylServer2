#include "../Common/iSingleton.h"
#include "../Common/Logger.h"

#include <string>

struct GlobalConfTable: Common::Singleton<GlobalConfTable>
{
	MAKE_SINGLETON(GlobalConfTable);
	
	Common::Logger::LogFile_ptr LogFile;
	uint32 PatchVersion;
	std::string PatchAddress;
	std::string LauncherListenAddress;
	std::string LauncherListenPort;
	std::string AuthServerListenAddress;
	std::string AuthServerListenPort;
	std::string GlobalUser;
	std::string GlobalPass;
	std::string LauncherListen;
private:
	GlobalConfTable();
	~GlobalConfTable();
	GlobalConfTable(GlobalConfTable&);
};