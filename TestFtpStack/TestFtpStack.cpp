#include "TestFtpStack.h"

void PrintUsage( const char * pszProgram )
{
	printf( "[Usage] %s {ftp IP} {ftp port:21} {user id} {password} upload {remote folder path} {local file path}\n", pszProgram );
	printf( "        %s {ftp IP} {ftp port:21} {user id} {password} upload_folder {remote folder path} {local folder path}\n", pszProgram );
	printf( "        %s {ftp IP} {ftp port:21} {user id} {password} download {remote folder path} {remote filename} {local file path}\n", pszProgram );
	printf( "        %s {ftp IP} {ftp port:21} {user id} {password} get_folder\n", pszProgram );
	printf( "        %s {ftp IP} {ftp port:21} {user id} {password} create_folder {remote folder path}\n", pszProgram );
	printf( "        %s {ftp IP} {ftp port:21} {user id} {password} delete_folder {remote folder path}\n", pszProgram );
	printf( "        %s {ftp IP} {ftp port:21} {user id} {password} list\n", pszProgram );
}

int main( int argc, char * argv[] )
{
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( argc < 6 )
	{
		PrintUsage( argv[0] );
		return 0;
	}

	const char * pszServerIp = argv[1];
	int iServerPort = atoi( argv[2] );
	const char * pszUserId = argv[3];
	const char * pszPassWord = argv[4];
	const char * pszCommand = argv[5];

	CFtpClient clsFtp;

	InitNetwork();
	CLog::SetLevel( LOG_DEBUG | LOG_NETWORK );

	// FTP ���� ���� �� �α���
	if( clsFtp.Connect( pszServerIp, iServerPort, true ) == false )
	{
		printf( "clsFtp.Connect(%s) error\n", pszServerIp );
		return 0;
	}

	if( clsFtp.Login( pszUserId, pszPassWord ) == false )
	{
		printf( "login error\n" );
		return 0;
	}

	if( !strcmp( pszCommand, "upload" ) )
	{
		// FTP ���ε�
		if( argc < 8 )
		{
			PrintUsage( argv[0] );
			return 0;
		}

		const char * pszRemoteFolder = argv[6];
		const char * pszLocalPath = argv[7];

		if( clsFtp.ChangeFolder( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.ChangeFolder(%s) error\n", pszRemoteFolder );
			return 0;
		}

		if( clsFtp.Upload( pszLocalPath ) == false )
		{
			printf( "clsFtp.Upload(%s) error\n", pszLocalPath );
			return 0;
		}
	}
	else if( !strcmp( pszCommand, "upload_folder" ) )
	{
		// ������ ���Ե� ��� ������ FTP ���ε�
		if( argc < 8 )
		{
			PrintUsage( argv[0] );
			return 0;
		}

		const char * pszRemoteFolder = argv[6];
		const char * pszLocalFolder = argv[7];
		FILE_LIST clsFileList;
		FILE_LIST::iterator itFL;

		if( clsFtp.ChangeFolder( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.ChangeFolder(%s) error\n", pszRemoteFolder );
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
	else if( !strcmp( pszCommand, "download" ) )
	{
		// FTP �ٿ�ε�
		if( argc < 9 )
		{
			PrintUsage( argv[0] );
			return 0;
		}

		const char * pszRemoteFolder = argv[6];
		const char * pszRemoteFile = argv[7];
		const char * pszLocalPath = argv[8];

		if( clsFtp.ChangeFolder( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.ChangeFolder(%s) error\n", pszRemoteFolder );
			return 0;
		}

		if( clsFtp.Download( pszRemoteFile, pszLocalPath ) == false )
		{
			printf( "clsFtp.Download(%s) error\n", pszRemoteFile );
			return 0;
		}
	}
	else if( !strcmp( pszCommand, "get_folder" ) )
	{
		// FTP ���� ���� ��������
		std::string strPath;

		if( clsFtp.GetCurrentFolder( strPath ) == false )
		{
			printf( "clsFtp.GetCurrentFolder() error\n" );
			return 0;
		}

		printf( "current directory[%s]\n", strPath.c_str() );
	}
	else if( !strcmp( pszCommand, "create_folder" ) )
	{
		// FTP ���� �����ϱ�
		if( argc < 7 )
		{
			PrintUsage( argv[0] );
			return 0;
		}

		const char * pszRemoteFolder = argv[6];

		if( clsFtp.CreateFolder( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.CreateFolder(%s) error\n", pszRemoteFolder );
			return 0;
		}

		if( clsFtp.ChangeFolder( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.ChangeFolder(%s) error\n", pszRemoteFolder );
			return 0;
		}

		std::string strPath;

		if( clsFtp.GetCurrentFolder( strPath ) == false )
		{
			printf( "clsFtp.GetCurrentFolder() error\n" );
			return 0;
		}

		printf( "current directory[%s]\n", strPath.c_str() );
	}
	else if( !strcmp( pszCommand, "delete_folder" ) )
	{
		// FTP ���� �����ϱ�
		if( argc < 7 )
		{
			PrintUsage( argv[0] );
			return 0;
		}

		const char * pszRemoteFolder = argv[6];

		if( clsFtp.DeleteFolder( pszRemoteFolder ) == false )
		{
			printf( "clsFtp.DeleteFolder(%s) error\n", pszRemoteFolder );
			return 0;
		}
	}
	else if( !strcmp( pszCommand, "list" ) )
	{
		if( argc >= 7 )
		{
			const char * pszRemoteFolder = argv[6];

			if( clsFtp.ChangeFolder( pszRemoteFolder ) == false )
			{
				printf( "clsFtp.ChangeFolder(%s) error\n", pszRemoteFolder );
				return 0;
			}
		}

		// FTP ������ ���Ե� ���� ����Ʈ ����ϱ�
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
	}

	return 0;
}
