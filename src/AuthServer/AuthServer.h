#ifndef _AUTHSERVER_H_
#define	_AUTHSERVER_H_

#include "../Common/stdDefs.h"
#include "../Common/typedef.h"
#include "../Common/Logger.h"
#include "../Common/iStartupClass.h"

#include <vector>
#include <string>

class AuthServer : public StartupClass {
public:
    AuthServer(vector<string> params);
    virtual ~AuthServer();
    int Start();
    void Stop();
    bool LoadConfig();
protected:
	uint8 m_serverSlot;
	string m_serverName;
	string m_globalUser;
	string m_globalPass;
	uint32 m_version;
	uint32 m_checkSum;
	uint8 m_serverGroup;
	common::shared::Logger::LogFile_ptr m_LogFile;
	//LoginServer m_loginServer;
};

#endif	/* _AUTHSERVER_H_ */