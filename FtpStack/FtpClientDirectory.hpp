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
 * @brief FTP ������ ���丮�� �����Ѵ�.
 * @param pszPath ���丮 PATH
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief FTP ������ ���� ���丮�� �����´�.
 * @param strPath [out] ���� ���丮�� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief FTP ������ ���ο� ���丮�� �����Ѵ�.
 * @param pszPath ������ ���丮 PATH
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief FTP �������� ���丮�� �����Ѵ�.
 * @param pszPath ������ ���丮 PATH
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
