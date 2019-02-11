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
 * @brief FTP 응답 메시지를 파싱하여서 저장하는 클래스
 */
class CFtpResponse
{
public:
	CFtpResponse();
	~CFtpResponse();

	int Parse( const char * pszText, int iTextLen );
	int ParseLine( const char * pszText, int iTextLen, bool & bLastLine );

	bool GetIpPort( std::string & strIp, int & iPort );

	/** FTP 응답 코드 */
	int m_iCode;

	/** FTP 응답 메시지 리스트 */
	STRING_LIST m_clsReplyList;
};

#endif
