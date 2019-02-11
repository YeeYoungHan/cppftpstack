/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "FileUtility.h"
#include "SipTcp.h"
#include "Log.h"
#include "TimeString.h"
#include <sys/stat.h>

#ifndef WIN32
#include <utime.h>
#endif

#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ������ �����ϴ��� �˻��Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsExistFile( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ũ�⸦ �����Ѵ�.
 * @param pszFileName ���� �̸�
 * @returns ������ �����ϸ� ���� ũ�⸦ �����ϰ� �׷��� ������ 0 �� �����Ѵ�.
 */
int64_t GetFileSize( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return 0;
	}

	return sttStat.st_size;
}

/**
 * @ingroup SipPlatform
 * @brief ���� �̸����� ���� Ȯ���ڸ� �����´�.
 * @param pszFileName ���� �̸�
 * @param strExt			���� Ȯ���� ���� ����
 * @returns ���� Ȯ���ڰ� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool GetFileExt( const char * pszFileName, std::string & strExt )
{
	int iLen = (int)strlen( pszFileName );

	strExt.clear();

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( pszFileName[i] == '.' )
		{
			strExt = pszFileName + i + 1;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ��ο��� ���� ��� ��������
 * @param pszFilePath	���� ���
 * @param strFolder		���� ��� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool GetFolderPathOfFilePath( const char * pszFilePath, std::string & strFolder )
{
	strFolder.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

#ifdef WIN32
	if( pszFilePath[iLen-1] == '\\' ) return false;
#else
	if( pszFilePath[iLen-1] == '/' ) return false;
#endif

	for( int i = iLen - 2; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{

#ifndef WIN32
			if( i == 0 )
			{
				strFolder = "/";
				return true;
			}
#endif

			strFolder.append( pszFilePath, i );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ��ο��� ���� �̸� ��������
 * @param pszFilePath ���� ���
 * @param strFileName ���� �̸� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool GetFileNameOfFilePath( const char * pszFilePath, std::string & strFileName )
{
	strFileName.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

#ifdef WIN32
	if( pszFilePath[iLen-1] == '\\' ) return false;
#else
	if( pszFilePath[iLen-1] == '/' ) return false;
#endif

	for( int i = iLen - 2; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{
			strFileName.append( pszFilePath + i + 1 );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ������ ���� �ð��� �����´�.
 * @param pszFileName ���� �̸�
 * @param strDate			[out] ���� ���� �ð� ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool GetFileDate( const char * pszFileName, std::string & strDate )
{
	strDate.clear();

	if( pszFileName == NULL ) return false;

	struct stat sttStat;
	char szDate[21];

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	GetDateTimeString( sttStat.st_mtime, szDate, sizeof(szDate) );
	strDate = szDate;

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ������ ���� �ð��� �����Ѵ�.
 * @param pszFileName ���� �̸�
 * @param pszDate			���� ���� �ð�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SetFileDate( const char * pszFileName, const char * pszDate )
{
	if( pszFileName == NULL || pszDate == NULL ) return false;
	if( strlen( pszDate ) != 14 ) return false;

	time_t iTime = ParseDateTimeString( pszDate );

#ifdef WIN32
	LONGLONG ll;
	FILETIME sttTime;

	ll = Int32x32To64( iTime, 10000000 ) + 116444736000000000;
	sttTime.dwLowDateTime = (DWORD)ll;
	sttTime.dwHighDateTime = ll >> 32;
	
	HANDLE hFile = CreateFile( pszFileName, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE ) return false;

	SetFileTime( hFile, NULL, NULL, &sttTime );
	CloseHandle( hFile );
#else
	struct utimbuf sttTime;

	sttTime.actime = iTime;
	sttTime.modtime = iTime;
	
	if( utime( pszFileName, &sttTime ) == -1 ) return false;
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ������ �����Ѵ�.
 * @param pszFileName ���� �̸�
 */
void DelFile( const char * pszFileName )
{
#ifdef WIN32
	DeleteFile( pszFileName );
#else
	unlink( pszFileName );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief ������ �����Ѵ�.
 * @param pszSrcFileName	���� ���� path
 * @param pszDestFileName ���纻 ���� path
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CpFile( const char * pszSrcFileName, const char * pszDestFileName )
{
	FILE * fdSrc = fopen( pszSrcFileName, "rb" );
	if( fdSrc == NULL )
	{
		CLog::Print( LOG_ERROR, "%s src file(%s) open error(%d)", __FUNCTION__, pszSrcFileName, GetError() );
		return false;
	}

	FILE * fdDest = fopen( pszDestFileName, "wb" );
	if( fdDest == NULL )
	{
		fclose( fdSrc );
		CLog::Print( LOG_ERROR, "%s dest file(%s) open error(%d)", __FUNCTION__, pszDestFileName, GetError() );
		return false;
	}

	size_t iRead;
	char szBuf[8192];
	bool bError = false;

	while( 1 )
	{
		iRead = fread( szBuf, 1, sizeof(szBuf), fdSrc );
		if( iRead <= 0 ) break;

		if( fwrite( szBuf, 1, iRead, fdDest ) != iRead )
		{
			CLog::Print( LOG_ERROR, "%s dest file(%s) write error(%d)", __FUNCTION__, pszDestFileName, GetError() );
			bError = true;
			break;
		}
	}

	fclose( fdDest );
	fclose( fdSrc );

	if( bError )
	{
		DelFile( pszDestFileName );
		return false;
	}

	return true;
}
