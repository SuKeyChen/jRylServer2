#include "../Common/iStartupClass.h"

#include <winsock2.h>
#include <windows.h>

#include <vector>
#include <string>

#include <Poco/Util/ServerApplication.h>

class LoginServer: public  Poco::Util::ServerApplication{
public:	
	LoginServer();
	~LoginServer();
private:
	void initialize(Poco::Util::Application& self);
	void uninitialize();
	int main(const std::vector<std::string>& args);
};