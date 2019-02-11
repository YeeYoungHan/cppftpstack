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

#ifndef _FTP_CLIENT_H_
#define _FTP_CLIENT_H_

#include "SipTcp.h"
#include "FtpResponse.h"

/**
 * @ingroup FtpStack
 * @brief FTP 클라이언트 클래스
 */
class CFtpClient
{
public:
	CFtpClient();
	~CFtpClient();

	bool Connect( const char * pszServerIp, int iServerPort = 21, bool bUseUtf8 = false );
	void Close();

	bool Login( const char * pszUserId, const char * pszPassWord );
	bool ChangeDirectory( const char * pszPath );
	bool Upload( const char * pszLocalPath );
	bool Download( const char * pszFileName, const char * pszLocalPath );

private:
	bool SendBinaryPassive( );
	bool Send( const char * fmt, ... );
	bool Recv( CFtpResponse & clsResponse, int iWantCode = 0 );
	bool Recv( int iWantCode );

	Socket			m_hSocket;
	std::string m_strServerIp;
	int					m_iServerPort;
	int					m_iTimeout;
	bool				m_bUseUtf8;

	std::string m_strRecvBuf;
	std::string m_strDataIp;
	int					m_iDataPort;
};

#endif
