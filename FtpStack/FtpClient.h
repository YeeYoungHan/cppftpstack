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

class CFtpClient
{
public:
	CFtpClient();
	~CFtpClient();

	bool Connect( const char * pszServerIp, int iServerPort = 21 );
	void Close();

	bool Login( const char * pszUserId, const char * pszPassWord );

private:
	bool Send( const char * pszSend );
	bool Recv( std::string & strRecv );

	Socket			m_hSocket;
	std::string m_strServerIp;
	int					m_iServerPort;
};

#endif
