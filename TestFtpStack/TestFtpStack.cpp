#include "TestFtpStack.h"

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		printf( "[Usage] %s {ftp IP} {user id} {password}\n", argv[0] );
		return 0;
	}

	const char * pszServerIp = argv[1];
	const char * pszUserId = argv[2];
	const char * pszPassWord = argv[3];

	CFtpClient clsFtp;

	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	if( clsFtp.Connect( pszServerIp ) == false )
	{
		printf( "clsFtp.Connect(%s) error\n", pszServerIp );
	}
	else
	{
		if( clsFtp.Login( pszUserId, pszPassWord ) == false )
		{
			printf( "login error\n" );
		}
		else
		{

		}
	}

	return 0;
}
