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
 * @brief FTP 서버의 현재 폴더에 존재하는 파일/폴더 리스트를 가져온다.
 * @param clsList [out] 파일/폴더 리스트 저장 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::List( FTP_FILE_LIST & clsList )
{
	clsList.clear();

	if( SendBinaryPassive() == false )
	{
		return false;
	}

	if( Send( "LIST" ) == false )
	{
		return false;
	}

	Socket hSocket = TcpConnect( m_strDataIp.c_str(), m_iDataPort, m_iTimeout );
	if( hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error(%d)", __FUNCTION__, m_strDataIp.c_str(), m_iDataPort, GetError() );
		return false;
	}

	int iRead;
	char szBuf[8192];
	std::string strBuf;

	while( 1 )
	{
		iRead = TcpRecv( hSocket, szBuf, sizeof(szBuf), m_iTimeout );
		if( iRead <= 0 ) break;

		strBuf.append( szBuf, iRead );
	}

	closesocket( hSocket );

	if( Recv( 150 ) == false ||
			Recv( 226 ) == false )
	{
		return false;
	}

	const char * pszBuf = strBuf.c_str();
	int iLen = strBuf.length();
	int iPos = -1, iWordCount = 0;
	bool bRowStart = true, bWindow = false;
	CFtpFile clsFile;

	if( iLen >= 9 && pszBuf[8] == ' ' )
	{
		bWindow = true;
	}

	for( int i = 0; i < iLen; ++i )
	{
		if( pszBuf[i] == '\r' || pszBuf[i] == '\n' )
		{
			if( iPos != -1 )
			{
				clsFile.m_strFileName.append( pszBuf + iPos, i - iPos );

#ifdef WIN32
				if( m_bUseUtf8 )
				{
					std::string strAnsi;

					if( Utf8ToAnsi( clsFile.m_strFileName.c_str(), strAnsi ) )
					{
						clsFile.m_strFileName = strAnsi;
					}
				}
#endif

				clsList.push_back( clsFile );
				clsFile.Clear();
				iPos = -1;
				iWordCount = 0;
			}

			bRowStart = true;
		}
		else if( pszBuf[i] == ' ' )
		{
			if( i >= 1 && pszBuf[i-1] != ' ' )
			{
				++iWordCount;
			}

			if( bWindow )
			{
				if( iWordCount == 3 )
				{
					iPos = i + 1;
				}
			}
			else
			{
				if( iWordCount == 8 )
				{
					iPos = i + 1;
				}
			}
		}
		else if( pszBuf[i] == '<' )
		{
			// IIS FTP 에서 폴더를 구분하기 위한 기능
			if( i + 5 < iLen && !strncmp( pszBuf + i, "<DIR>", 5 ) )
			{
				clsFile.m_bFolder = true;
			}
		}
		else if( bRowStart )
		{
			if( pszBuf[i] == 'd' )
			{
				clsFile.m_bFolder = true;
			}

			bRowStart = false;
		}
	}

	return true;
}
