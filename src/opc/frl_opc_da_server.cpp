#include "frl_platform.h"
#if( FRL_PLATFORM == FRL_PLATFORM_WIN32 )
#include <vector>
#include "opc/frl_opc_da_server.h"
#include "../dependency/vendors/opc_foundation/opcda.h"
#include "opc/frl_opc_server_factory.h"

namespace frl{ namespace opc{

DAServer::DAServer( const ServerType& serverType )
	:	ServerKind( serverType )
{
	isInit = False;
}

DAServer::~DAServer()
{
	if ( isInit )
	{
		CoRevokeClassObject( objectId );
		CoUninitialize();
	}
}

void DAServer::registerServer()
{
	std::vector< IID > categories;
	categories.push_back( ::CATID_OPCDAServer20 );
	ServerKind::registerServer( categories );
}

void DAServer::registerServer3()
{
	std::vector< IID > categories;
	categories.push_back( ::CATID_OPCDAServer20 );
	categories.push_back( ::CATID_OPCDAServer30 );
	ServerKind::registerServer( categories );
}

HRESULT DAServer::init()
{
	HRESULT hResult = S_OK;
	hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hResult))
	{
		isInit = False;
		return hResult;
	}

	factory.isOutProc( True );

	hResult = ::CoRegisterClassObject( lexicalCast<frl::String,CLSID>( getCLSID() ), &factory, CLSCTX_LOCAL_SERVER |
		CLSCTX_REMOTE_SERVER |
		CLSCTX_INPROC_SERVER,
		REGCLS_MULTIPLEUSE, &objectId );

	if( FAILED(hResult) )
	{
		CoUninitialize();
		isInit = False;
		return hResult;
	}			
	isInit = True;
	return hResult;
}

void DAServer::unregisterServer()
{
	ServerKind::unregisterServer();
}

} // namespace opc
} // FatRat Library

#endif // FRL_PLATFORM_WIN32
