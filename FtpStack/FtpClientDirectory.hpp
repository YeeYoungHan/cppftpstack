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
 * @brief FTP 서버의 디렉토리를 변경한다.
 * @param pszPath 디렉토리 PATH
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::ChangeFolder( const char * pszPath )
{
	if( Send( "CWD %s", pszPath ) == false || 
			Recv( 250 ) == false )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버의 현재 디렉토리를 가져온다.
 * @param strPath [out] 현재 디렉토리를 저장할 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::GetCurrentFolder( std::string & strPath )
{
	CFtpResponse clsRes;

	strPath.clear();

	if( Send( "PWD" ) == false || 
			Recv( clsRes, 257 ) == false )
	{
		return false;
	}

	STRING_LIST::iterator itSL = clsRes.m_clsReplyList.begin();
	if( itSL == clsRes.m_clsReplyList.end() )
	{
		return false;
	}

	const char * pszLine = itSL->c_str();
	int iLineLen = itSL->length();
	int iStartPos = -1;

	for( int i = 0; i < iLineLen; ++i )
	{
		if( pszLine[i] == '"' )
		{
			if( iStartPos == -1 )
			{
				iStartPos = i + 1;
			}
			else
			{
				strPath.append( pszLine + iStartPos, i - iStartPos );
				return true;
			}
		}
	}

	return false;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버에 새로운 디렉토리를 생성한다.
 * @param pszPath 생성할 디렉토리 PATH
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::CreateFolder( const char * pszPath )
{
	if( Send( "MKD %s", pszPath ) == false || 
			Recv( 257 ) == false )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버에서 디렉토리를 삭제한다.
 * @param pszPath 삭제할 디렉토리 PATH
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::DeleteFolder( const char * pszPath )
{
	if( Send( "RMD %s", pszPath ) == false || 
			Recv( 250 ) == false )
	{
		return false;
	}

	return true;
}
