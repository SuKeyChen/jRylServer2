#include "LoginServer.h"

#include "../Common/packet/pktUserLogin.h"

#define STARTUP_CLASS LoginServer
#define MODULE_NAME "LoginServer"
#include "../Common/main.h"

#define CreateEvent CreateEventA

#include "../Common/network/SocketClient.h"


LoginServer::LoginServer()
{
	
}

LoginServer::~LoginServer()
{

}

void LoginServer::initialize(Poco::Util::Application& self)
{
}

void LoginServer::uninitialize()
{
}

int LoginServer::main(const std::vector<std::string>& args)
{
	Common::network::SocketClient client1;
	Common::network::SocketClient client2;
	Common::network::SocketClient client3;
	Common::network::SocketClient client4;
	Common::network::SocketClient client5;
	Common::network::SocketClient client6;
	Common::network::SocketClient client7;
	Common::network::SocketClient client8;
	Common::network::SocketClient client9;
	Common::network::SocketClient client10;
	Common::network::SocketClient client12;
	Common::network::SocketClient clien13;
	Common::network::SocketClient client14;
	Common::network::SocketClient client15;
	Common::network::SocketClient client16;
	Common::network::SocketClient client17;
	Common::network::SocketClient client18;
	Common::network::SocketClient client19;
	Common::network::SocketClient client21;
	Common::network::SocketClient client22;
	Common::network::SocketClient client23;
	Common::network::SocketClient client24;
	Common::network::SocketClient client25;
	Common::network::SocketClient client26;
	Common::network::SocketClient client27;
	Common::network::SocketClient client28;
	Common::network::SocketClient client29;
	Common::network::SocketClient clien1t1;
	Common::network::SocketClient clien2t2;
	Common::network::SocketClient clien2t3;
	Common::network::SocketClient clien2t4;
	Common::network::SocketClient clien2t5;
	Common::network::SocketClient clien2t6;
	Common::network::SocketClient clien2t7;
	Common::network::SocketClient clien2t8;
	Common::network::SocketClient clien2t9;
	Common::network::SocketClient client210;
	Common::network::SocketClient client212;
	Common::network::SocketClient clien123;
	Common::network::SocketClient client2214;
	Common::network::SocketClient client215;
	Common::network::SocketClient client216;
	Common::network::SocketClient client217;
	Common::network::SocketClient client128;
	Common::network::SocketClient client129;
	Common::network::SocketClient client221;
	Common::network::SocketClient client2222;
	Common::network::SocketClient client223;
	Common::network::SocketClient client224;
	Common::network::SocketClient clie2nt25;
	Common::network::SocketClient clie2nt26;
	Common::network::SocketClient clie2nt27;
	Common::network::SocketClient clien2t28;
	Common::network::SocketClient clien22t29;

	Common::network::SocketClient clieasdnt1;
	Common::network::SocketClient cliasdent2;
	Common::network::SocketClient cliesadnt3;
	Common::network::SocketClient cliasdent4;
	Common::network::SocketClient clienasdt5;
	Common::network::SocketClient client6as;
	Common::network::SocketClient cliesant7;
	Common::network::SocketClient clientasdd8;
	Common::network::SocketClient clieasdnt9;
	Common::network::SocketClient clientasd10;
	Common::network::SocketClient clsient12;
	Common::network::SocketClient clisen13;
	Common::network::SocketClient cliesnt14;
	Common::network::SocketClient clienst15;
	Common::network::SocketClient clients16;
	Common::network::SocketClient cslient17;
	Common::network::SocketClient clsient18;
	Common::network::SocketClient clisent19;
	Common::network::SocketClient cliesnt21;
	Common::network::SocketClient clienst22;
	Common::network::SocketClient clients23;
	Common::network::SocketClient client2s4;
	Common::network::SocketClient client25s;
	Common::network::SocketClient sclient26;
	Common::network::SocketClient cslient27;
	Common::network::SocketClient clsient28;
	Common::network::SocketClient clisent29;
	Common::network::SocketClient cliesn1t1;
	Common::network::SocketClient cliens2t2;
	Common::network::SocketClient clien2st3;
	Common::network::SocketClient clien2ts4;
	Common::network::SocketClient clien2t5s;
	Common::network::SocketClient clien2ts6;
	Common::network::SocketClient clien2ts7;
	Common::network::SocketClient clien2ts8;
	Common::network::SocketClient clienss2t9;
	Common::network::SocketClient client2s10;
	Common::network::SocketClient cliesnst212;
	Common::network::SocketClient clien12s3;
	Common::network::SocketClient clients2214;
	Common::network::SocketClient cliensst215;
	Common::network::SocketClient clienst216;
	Common::network::SocketClient cliasdent217;
	Common::network::SocketClient clieasdnt128;
	Common::network::SocketClient cliasdent129;
	Common::network::SocketClient cliasdent221;
	Common::network::SocketClient cliasdent2222;
	Common::network::SocketClient cliasdent223;
	Common::network::SocketClient clasdient224;
	Common::network::SocketClient cliasdasde2nt25;
	Common::network::SocketClient cliasde2nt26;
	Common::network::SocketClient cliassadde2nt27;
	Common::network::SocketClient clieasdn2t28;
	Common::network::SocketClient clieasdn22t29;

	Common::network::SocketClient casdlasdient1;
	Common::network::SocketClient asd;
	Common::network::SocketClient cliasdentasd4;
	Common::network::SocketClient cliasdentasd5;
	Common::network::SocketClient clisadentasdsad6;
	Common::network::SocketClient cliasdasdsadent7;
	Common::network::SocketClient clieasdnasdsat8;
	Common::network::SocketClient asdasdsadclient9;
	Common::network::SocketClient cliesasdadsant10;
	Common::network::SocketClient casdasdlient12;
	Common::network::SocketClient clasdsaien13;
	Common::network::SocketClient cliesadnat14;
	Common::network::SocketClient clasdieant15;
	Common::network::SocketClient clieant16;
	Common::network::SocketClient clieant17;
	Common::network::SocketClient clienat18;
	Common::network::SocketClient clieant19;
	Common::network::SocketClient clieant21;
	Common::network::SocketClient clieant22;
	Common::network::SocketClient clieant23;
	Common::network::SocketClient clieant24;
	Common::network::SocketClient clieant25;
	Common::network::SocketClient clieant26;
	Common::network::SocketClient clieant27;
	Common::network::SocketClient clieant28;
	Common::network::SocketClient clieant29;
	Common::network::SocketClient cliean1t1;
	Common::network::SocketClient cliean2t2;
	Common::network::SocketClient cliean2t3;
	Common::network::SocketClient cliean2t4;
	Common::network::SocketClient cliean2t5;
	Common::network::SocketClient cliean2t6;
	Common::network::SocketClient cliena2t7;
	Common::network::SocketClient cliena2t8;
	Common::network::SocketClient cliena2t9;
	Common::network::SocketClient clienat210;
	Common::network::SocketClient clienat212;
	Common::network::SocketClient clienaa123;
	Common::network::SocketClient clienat2214;
	Common::network::SocketClient clienat215;
	Common::network::SocketClient clienat216;
	Common::network::SocketClient clienat217;
	Common::network::SocketClient clienat128;
	Common::network::SocketClient clienat129;
	Common::network::SocketClient clienat221;
	Common::network::SocketClient clienat2222;
	Common::network::SocketClient clienat223;
	Common::network::SocketClient clienat224;
	Common::network::SocketClient clie2ant25;
	Common::network::SocketClient clie2ant26;
	Common::network::SocketClient clie2ant27;
	Common::network::SocketClient cliena2t28;
	Common::network::SocketClient cliena22t29;

	Common::network::SocketClient claient1;
	Common::network::SocketClient clisent2;
	Common::network::SocketClient cliesnt3;
	Common::network::SocketClient clienst4;
	Common::network::SocketClient clientss5;
	Common::network::SocketClient clienst6;
	Common::network::SocketClient clienst7;
	Common::network::SocketClient clienst8;
	Common::network::SocketClient clienst9;
	Common::network::SocketClient clienst10;
	Common::network::SocketClient clienst12;
	Common::network::SocketClient cliens13;
	Common::network::SocketClient clienst14;
	Common::network::SocketClient cliasdenst15;
	Common::network::SocketClient clienasdst16;
	Common::network::SocketClient cliasdenst17;
	Common::network::SocketClient clasdienst18;
	Common::network::SocketClient clisadenst19;
	Common::network::SocketClient clisadenst21;
	Common::network::SocketClient cliasdenst22;
	Common::network::SocketClient clienst23;
	Common::network::SocketClient clienst24;
	Common::network::SocketClient clienst25;
	Common::network::SocketClient clienst26;
	Common::network::SocketClient clienst27;
	Common::network::SocketClient clienst28;
	Common::network::SocketClient clienst29;
	Common::network::SocketClient cliens1t1;
	Common::network::SocketClient clieasdns2t2;
	Common::network::SocketClient cliens2t3;
	Common::network::SocketClient cliens2t4;
	Common::network::SocketClient cliens2t5;
	Common::network::SocketClient cliens2t6;
	Common::network::SocketClient cliens2t7;
	Common::network::SocketClient cliens2t8;
	Common::network::SocketClient cliens2t9;
	Common::network::SocketClient clienst210;
	Common::network::SocketClient clienst212;
	Common::network::SocketClient cliens123;
	Common::network::SocketClient clienst2214;
	Common::network::SocketClient clienst215;
	Common::network::SocketClient casdlienst216;
	Common::network::SocketClient clienst217;
	Common::network::SocketClient clienst128;
	Common::network::SocketClient clienst129;
	Common::network::SocketClient clienst221;
	Common::network::SocketClient clienst2222;
	Common::network::SocketClient clienst223;
	Common::network::SocketClient clienst224;
	Common::network::SocketClient clie2snt25;
	Common::network::SocketClient clie2snt26;
	Common::network::SocketClient clie2snt27;
	Common::network::SocketClient cliens2t28;
	Common::network::SocketClient clieasdsadn22t29;
	
	waitForTerminationRequest();
	return 0;
}