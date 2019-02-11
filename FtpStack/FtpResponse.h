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

#ifndef _FTP_RESPONSE_H_
#define _FTP_RESPONSE_H_

#include "StringUtility.h"

/**
 * @ingroup FtpStack
 * @brief FTP ���� �޽����� �Ľ��Ͽ��� �����ϴ� Ŭ����
 */
class CFtpResponse
{
public:
	CFtpResponse();
	~CFtpResponse();

	int Parse( const char * pszText, int iTextLen );
	int ParseLine( const char * pszText, int iTextLen, bool & bLastLine );

	bool GetIpPort( std::string & strIp, int & iPort );

	/** FTP ���� �ڵ� */
	int m_iCode;

	/** FTP ���� �޽��� ����Ʈ */
	STRING_LIST m_clsReplyList;
};

#endif
