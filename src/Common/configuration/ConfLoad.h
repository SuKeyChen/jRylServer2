#ifndef CONFLOAD_H
#define	CONFLOAD_H

#include <boost/static_assert.hpp>

#include "iniparser.h"
#include "../typedef.h"
#include "../utils.h"

#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

namespace Common {
namespace configuration {

class ConfLoad {
public:
    ConfLoad();
    bool LoadConfig(const char* file);
    virtual ~ConfLoad();
    
    bool GetBool(const char* section, const char* key, bool &ret);
    bool GetString(const char* section, const char* key, string &ret);
    int GetBytes(const char* section, const char* key, byte* ret);
    template<class T> bool GetInt(const char* section, const char* key, T& ret);
	template<class T> bool GetUInt(const char* section, const char* key, T& ret);
	template<class T> bool GetFloat(const char* section, const char* key, T& ret);
private:
    dictionary *m_dic;
};

template<class T> bool ConfLoad::GetInt(const char* section, const char* key, T& ret) {
	if (m_dic == NULL) {
        return false;
    }
	char* str;
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if(str = iniparser_getstring(m_dic, seckey, NULL)) {
		ret = j_atoi<T>(str);
		return true;
	}
	return false;
}

template<class T> bool ConfLoad::GetUInt(const char* section, const char* key, T& ret) {
	if (m_dic == NULL) {
        return false;
    }
	char* str;
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if(str = iniparser_getstring(m_dic, seckey, NULL)) {
		ret = j_atoui<T>(str);
		return true;
	}
	return false;
}

template<class T> bool ConfLoad::GetFloat(const char* section, const char* key, T& ret) {
	if (m_dic == NULL) {
        return false;
    }
	char* str;
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if(str = iniparser_getstring(m_dic, seckey, NULL)) {
		ret = j_atof<T>(str);
		return true;
	}
	return false;
}

} //namespace configuration
} //namespace Common
#endif	/* CONFLOAD_H */
