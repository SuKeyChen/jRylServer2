#ifndef _ISTARTUP_CLASS_H_
#define _ISTARTUP_CLASS_H_

namespace Common {

class StartupClass {
public:
    virtual bool LoadConfig() = 0;
    virtual int Start() = 0;
    virtual void Stop() = 0;
    virtual ~StartupClass() {};
};
} //namespace common

#endif