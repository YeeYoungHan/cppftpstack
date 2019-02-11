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

#include "FtpResponse.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CFtpResponse::CFtpResponse() : m_iCode(0)
{
}

CFtpResponse::~CFtpResponse()
{
}

/**
 * @ingroup FtpStack
 * @brief FTP ���� �޽����� �Ľ��Ͽ��� ���� �ڷᱸ���� �����Ѵ�.
 * @param pszText		FTP ���� �޽���
 * @param iTextLen	FTP ���� �޽��� ����
 * @returns �����ϸ� �Ľ̵� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CFtpResponse::Parse( const char * pszText, int iTextLen )
{
	int iPos = 0, iLen;
	bool bLastLine = false;

	m_iCode = 0;
	m_clsReplyList.clear();

	while( iPos < iTextLen )
	{
		iLen = ParseLine( pszText + iPos, iTextLen - iPos, bLastLine );
		if( iLen == -1 ) return -1;
		iPos += iLen;
		
		if( bLastLine ) break;
	}

	return iPos;
}

/**
 * @ingroup FtpStack
 * @brief FTP ���� �޽����� 1������ �Ľ��Ͽ��� ���� �ڷᱸ���� �����Ѵ�.
 * @param pszText		FTP ���� �޽���
 * @param iTextLen	FTP ���� �޽��� ����
 * @param	bLastLine	[out] ������ �����̸� �� �޼ҵ� ȣ����, true �� ����ǰ� �׷��� ������ false �� ����ȴ�.
 * @returns �����ϸ� �Ľ̵� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CFtpResponse::ParseLine( const char * pszText, int iTextLen, bool & bLastLine )
{
	// reply code(3byte) space(1byte) CRLF(2byte)
	if( iTextLen < 6 ) 
	{
		// code(3byte) CRLF(2byte)
		if( iTextLen == 5 )
		{
			for( int i = 0; i < 3; ++i )
			{
				if( isdigit( pszText[i] ) == 0 ) return -1;
			}

			if( pszText[3] == '\r' && pszText[4] == '\n' )
			{
				bLastLine = true;

				std::string strCode;
				strCode.append( pszText, 3 );

				m_iCode = atoi( strCode.c_str() );

				return iTextLen;
			}
		}

		return -1;
	}

	for( int i = 0; i < 3; ++i )
	{
		if( isdigit( pszText[i] ) == 0 ) return -1;
	}

	int iLen = -1;

	if( pszText[3] == ' ' )
	{
		bLastLine = true;
		
		std::string strCode;
		strCode.append( pszText, 3 );

		m_iCode = atoi( strCode.c_str() );
	}
	else
	{
		bLastLine = false;
	}

	for( int i = 5; i < iTextLen; ++i )
	{
		if( pszText[i-1] == '\r' && pszText[i] == '\n' )
		{
			std::string strLine;

			strLine.append( pszText + 4, i - 5 );
			m_clsReplyList.push_back( strLine );

			iLen = i + 1;
			break;
		}
	}

	return iLen;
}
