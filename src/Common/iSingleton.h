#ifndef SINGLETON_H
#define SINGLETON_H

#include "typedef.h"

#define MAKE_SINGLETON(CLASS) friend class Common::Singleton<CLASS>

namespace Common {

template<class T> class Singleton {
public: 
    static T& GetInstance();
    static T* GetInstance_ptr();
    static void DestroyInstance();
protected:
    inline Singleton() {
        Singleton::ms_Instance = static_cast<T*>(this); 
    }

    inline ~Singleton() {
        Singleton::ms_Instance = NULL; 
    }
private:
    static T* ms_Instance; 
    inline Singleton(Singleton const&) {}
    inline Singleton& operator=(Singleton const&) { return *this; }
};

template<class T> T& Singleton<T>::GetInstance() {
    if ( Singleton::ms_Instance == NULL ) {
        Singleton::ms_Instance = new T;
    }
    return *(Singleton::ms_Instance);
}

template<class T> T* Singleton<T>::GetInstance_ptr() {
    if ( Singleton::ms_Instance == NULL ) {
        Singleton::ms_Instance = new T;
    }
    return Singleton::ms_Instance;
}

template<class T> void Singleton<T>::DestroyInstance() {
    if ( Singleton::ms_Instance != NULL ) {
        delete Singleton::ms_Instance;
        Singleton::ms_Instance = NULL;
    }
}

template<class T> T* Singleton<T>::ms_Instance = NULL;


} //namespace common

#endif