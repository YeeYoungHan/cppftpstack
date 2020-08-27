#include "TestFtpStack2.h"

void PrintUsage( const char * pszProgram )
{
	printf( "[Usage] %s {ftp IP} {user id} {password}\n", pszProgram );
}

int main( int argc, char * argv[] )
{
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( argc < 4 )
	{
		PrintUsage( argv[0] );
		return 0;
	}

	const char * pszServerIp = argv[1];
	const char * pszUserId = argv[2];
	const char * pszPassWord = argv[3];

	CFtpClient clsFtp;

	InitNetwork();
	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	// FTP 서버 연결 및 로그인
	if( clsFtp.Connect( pszServerIp, 21, true ) == false )
	{
		printf( "clsFtp.Connect(%s) error\n", pszServerIp );
		return 0;
	}

	if( clsFtp.Login( pszUserId, pszPassWord ) == false )
	{
		printf( "login error\n" );
		return 0;
	}

	for( int i = 0; i < 100; ++i )
	{
		FTP_FILE_LIST clsList;
		FTP_FILE_LIST::iterator itFL;

		if( clsFtp.List( clsList ) == false )
		{
			printf( "clsFtp.List()\n" );
			return 0;
		}

		for( itFL = clsList.begin(); itFL != clsList.end(); ++itFL )
		{
			if( itFL->m_bFolder )
			{
				printf( "folder[%s]\n", itFL->m_strFileName.c_str() );
			}
			else
			{
				printf( "file[%s]\n", itFL->m_strFileName.c_str() );
			}
		}

		if( clsFtp.ChangeFolder( "temp" ) == false )
		{
			printf( "clsFtp.ChangeFolder error\n" );
			return 0;
		}

		if( clsFtp.ChangeFolder( ".." ) == false )
		{
			printf( "clsFtp.ChangeFolder error\n" );
			return 0;
		}
	}

	return 0;
}
