#ifndef _CRIPTENGINE_H_
#define	_CRIPTENGINE_H_

#include <cstdlib>

#include "../typedef.h"
#include "../Buffer.h"
#include "../iPackable.h"
#include "../iSingleton.h"
#include "SeedCipher.h"

namespace Common {
namespace crypto {

class CryptEngine: public  Singleton<CryptEngine>
{
MAKE_SINGLETON(CryptEngine);
public:
	struct Cryptkey : Packable {
	    uint8 Key1;
	    uint8 Key2;
	    uint16 CodePage;
	
	    Cryptkey() {
	        CryptEngine::GetInstance().InitKey(*this);
	    }
	
	    void Pack(Buffer_ptr buffer) {
	        buffer->Add(Key1);
	        buffer->Add(Key2);
	        buffer->Add(CodePage);
	    }
	
	    void Unpack(Buffer_ptr buffer) {
	        Key1 = buffer->Get<uint8>();
	        Key2 = buffer->Get<uint8>();
	        CodePage = buffer->Get<uint16>();
	    }
	};
    enum GGError {
        GGERRO_Sucess,
        GGERRO_ClientKey,
        GGERRO_Checksum = 4
    };

    

    void SetGGCryptParams(byte* GGClientSeedKey, byte* GGServerSeedKey, byte* GGKey);

    void InitKey(Cryptkey& k);
    void XorCrypt(Buffer_ptr buffer, Cryptkey& key);
    void XorDecrypt(Buffer_ptr buffer);
    
    void GGCrypt(Buffer_ptr buffer);
    GGError GGDecrypt(Buffer_ptr buffer);

    void XorCryptPacketHeader(byte* bytes);
    void XorCryptPacketBody(byte* bytes, size_t size, Cryptkey& key);
    void XorDecryptPacketHeader(byte* bytes);
    void XorDecryptPacketBody(byte* bytes, size_t size, Cryptkey& key);
private:
    CryptEngine();
	~CryptEngine();

    byte m_GGClientSeedKey[16];
    byte m_GGServerSeedKey[16];
    byte* m_GGKey;
    SeedCipher::SeedParam m_GGServerSeedParam;
    SeedCipher::SeedParam m_GGClientSeedParam;

    byte m_codePageCount;
    byte m_codePages[100];
};

} //namespace crypt
} //namespace common

#endif	/* _CRIPTENGINE_H_ */

