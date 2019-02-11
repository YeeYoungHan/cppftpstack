#include "TestFtpStack.h"

void PrintUsage( const char * pszProgram )
{
	printf( "[Usage] %s {ftp IP} {user id} {password} upload_folder {remote folder path} {local folder path}\n", pszProgram );
}

int main( int argc, char * argv[] )
{
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( argc <= 5 )
	{
		PrintUsage( argv[0] );
		return 0;
	}

	const char * pszServerIp = argv[1];
	const char * pszUserId = argv[2];
	const char * pszPassWord = argv[3];
	const char * pszCommand = argv[4];

	CFtpClient clsFtp;

	InitNetwork();
	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

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

	if( !strcmp( pszCommand, "upload_folder" ) )
	{
		if( argc <= 6 )
		{
			PrintUsage( argv[0] );
			return 0;
		}

		const char * pszRemoteFolder = argv[5];
		const char * pszLocalFolder = argv[6];
		FILE_LIST clsFileList;
		FILE_LIST::iterator itFL;

		if( clsFtp.ChangeDirectory( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.ChangeDirectory(%s) error\n", pszRemoteFolder );
			return 0;
		}

		CDirectory::FileList( pszLocalFolder, clsFileList );

		for( itFL = clsFileList.begin(); itFL != clsFileList.end(); ++itFL )
		{
			std::string strPath = pszLocalFolder;
			CDirectory::AppendName( strPath, itFL->c_str() );

			if( clsFtp.Upload( strPath.c_str() ) == false )
			{
				printf( "clsFtp.Upload(%s) error\n", strPath.c_str() );
				break;
			}
		}
	}

	return 0;
}
