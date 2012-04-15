#ifndef _LOGINSERVER_H_
#define _LOGINSERVER_H_

#include <vector>
#include <string>

#include <Poco/Util/ServerApplication.h>

class LoginServer: public  Poco::Util::ServerApplication {
public:
	LoginServer();
	~LoginServer();
private:
	void initialize(Poco::Util::Application& self);
	void uninitialize();
	int main(const std::vector<std::string>& args);
};

#endif