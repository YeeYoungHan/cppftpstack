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

/**
 * @ingroup FtpStack
 * @brief 파일을 FTP 서버로 업로드한다.
 * @param pszLocalPath 로컬 파일 PATH
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Upload( const char * pszLocalPath )
{
	std::string strFileName;

	if( IsExistFile( pszLocalPath ) == false )
	{
		CLog::Print( LOG_ERROR, "%s file(%s) doesn't exist", __FUNCTION__, pszLocalPath );
		return false;
	}

	if( GetFileNameOfFilePath( pszLocalPath, strFileName ) == false )
	{
		CLog::Print( LOG_ERROR, "%s GetFileNameOfFilePath(%s) error", __FUNCTION__, pszLocalPath );
		return false;
	}

#ifdef WIN32
	if( m_bUseUtf8 )
	{
		std::string strUtf8;

		if( AnsiToUtf8( strFileName.c_str(), strUtf8 ) )
		{
			strFileName = strUtf8;
		}
	}
#endif

	if( SendBinaryPassive() == false )
	{
		return false;
	}

	if( Send( "STOR %s", strFileName.c_str() ) == false )
	{
		return false;
	}

	Socket hSocket = TcpConnect( m_strDataIp.c_str(), m_iDataPort, m_iTimeout );
	if( hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error(%d)", __FUNCTION__, m_strDataIp.c_str(), m_iDataPort, GetError() );
		return false;
	}

	int iFd = open( pszLocalPath, OPEN_READ_FLAG, OPEN_READ_MODE );
	if( iFd >= 0 )
	{
		int iRead;
		char szBuf[8192];

		while( 1 )
		{
			iRead = read( iFd, szBuf, sizeof(szBuf) );
			if( iRead <= 0 ) break;

			if( TcpSend( hSocket, szBuf, iRead ) != iRead )
			{
				CLog::Print( LOG_ERROR, "%s TcpSend(%s:%d) error(%d)", __FUNCTION__, m_strDataIp.c_str(), m_iDataPort, GetError() );
				break;
			}
		}

		close( iFd );
	}
	else
	{
		CLog::Print( LOG_ERROR, "%s open(%s) error(%d)", __FUNCTION__, pszLocalPath, GetError() );
	}

	closesocket( hSocket );

	if( Recv( 150 ) == false ||
			Recv( 226 ) == false )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버에서 파일을 다운로드한다.
 * @param pszFileName		FTP 서버의 파일 이름
 * @param pszLocalPath	다운로드한 파일을 저장할 파일 PATH
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Download( const char * pszFileName, const char * pszLocalPath )
{
	std::string strFileName = pszFileName;

	if( IsExistFile( pszLocalPath ) )
	{
		CLog::Print( LOG_ERROR, "%s file(%s) exist", __FUNCTION__, pszLocalPath );
		return false;
	}

#ifdef WIN32
	if( m_bUseUtf8 )
	{
		std::string strUtf8;

		if( AnsiToUtf8( pszFileName, strUtf8 ) )
		{
			strFileName = strUtf8;
		}
	}
#endif

	if( SendBinaryPassive() == false )
	{
		return false;
	}

	if( Send( "RETR %s", strFileName.c_str() ) == false )
	{
		return false;
	}

	Socket hSocket = TcpConnect( m_strDataIp.c_str(), m_iDataPort, m_iTimeout );
	if( hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error(%d)", __FUNCTION__, m_strDataIp.c_str(), m_iDataPort, GetError() );
		return false;
	}

	int iFd = open( pszLocalPath, OPEN_WRITE_FLAG, OPEN_WRITE_MODE );
	if( iFd >= 0 )
	{
		int iRead;
		char szBuf[8192];

		while( 1 )
		{
			iRead = TcpRecv( hSocket, szBuf, sizeof(szBuf), m_iTimeout );
			if( iRead <= 0 ) break;

			if( write( iFd, szBuf, iRead ) != iRead )
			{
				CLog::Print( LOG_ERROR, "%s write error(%d)", __FUNCTION__, GetError() );
				break;
			}
		}

		close( iFd );
	}
	else
	{
		CLog::Print( LOG_ERROR, "%s open(%s) error(%d)", __FUNCTION__, pszLocalPath, GetError() );
	}

	closesocket( hSocket );

	if( Recv( 150 ) == false ||
			Recv( 226 ) == false )
	{
		return false;
	}

	return true;
}
