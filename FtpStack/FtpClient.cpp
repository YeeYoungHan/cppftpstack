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

#include "SipPlatformDefine.h"
#include "FtpClient.h"

CFtpClient::CFtpClient() : m_hSocket(INVALID_SOCKET), m_iServerPort(21), m_iTimeout(10)
{
	InitNetwork();
}

CFtpClient::~CFtpClient()
{
	Close();
}

bool CFtpClient::Connect( const char * pszServerIp, int iServerPort )
{
	if( m_hSocket != INVALID_SOCKET ) return false;

	m_hSocket = TcpConnect( pszServerIp, iServerPort );
	if( m_hSocket == INVALID_SOCKET )
	{
		return false;
	}

	m_strServerIp = pszServerIp;
	m_iServerPort = iServerPort;

	if( Recv( 220 ) == false )
	{
		Close();
		return false;
	}

	return true;
}

void CFtpClient::Close()
{
	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}
}

bool CFtpClient::Login( const char * pszUserId, const char * pszPassWord )
{
	if( Send( "USER %s", pszUserId ) == false )
	{
		return false;
	}

	if( Recv( 331 ) == false )
	{
		return false;
	}

	if( Send( "PASS %s", pszPassWord ) == false )
	{
		return false;
	}

	if( Recv( 230 ) == false )
	{
		return false;
	}

	return true;
}

bool CFtpClient::Send( const char * fmt, ... )
{
	va_list		ap;
	char			szSendBuf[8192];
	int				iSendLen;

	va_start( ap, fmt );
	iSendLen = vsnprintf( szSendBuf, sizeof(szSendBuf)-3, fmt, ap );
	va_end( ap );

	snprintf( szSendBuf + iSendLen, sizeof(szSendBuf) - iSendLen, "\r\n" );
	iSendLen += 2;
	
	if( TcpSend( m_hSocket, szSendBuf, iSendLen ) != iSendLen )
	{
		CLog::Print( LOG_ERROR, "%s TcpSend(%s:%d) [%.*s] error(%d)", __FUNCTION__, m_strServerIp.c_str(), m_iServerPort, iSendLen, szSendBuf, GetError() );
		return false;
	}

	CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%.*s]", m_strServerIp.c_str(), m_iServerPort, iSendLen, szSendBuf );

	return true;
}

bool CFtpClient::Recv( CFtpResponse & clsResponse, int iWantCode )
{
	std::string strRecvBuf;
	char	szRecvBuf[8192];
	int n;

	while( 1 )
	{
		n = TcpRecv( m_hSocket, szRecvBuf, sizeof(szRecvBuf), m_iTimeout );
		if( n <= 0 )
		{
			CLog::Print( LOG_ERROR, "%s TcpRecv(%s) error(%d)", __FUNCTION__, strRecvBuf.c_str(), GetError() );
			return false;
		}

		CLog::Print( LOG_NETWORK, "TcpRecv(%s:%d) [%.*s]", m_strServerIp.c_str(), m_iServerPort, n, szRecvBuf );

		strRecvBuf.append( szRecvBuf, n );
		if( clsResponse.Parse( strRecvBuf.c_str(), (int)strRecvBuf.length() ) > 0 )
		{
			break;
		}
	}

	if( iWantCode )
	{
		if( clsResponse.m_iCode != iWantCode )
		{
			CLog::Print( LOG_ERROR, "%s reply code(%d) != want code(%d)", __FUNCTION__, clsResponse.m_iCode, iWantCode );
			return false;
		}
	}

	return true;
}

bool CFtpClient::Recv( int iWantCode )
{
	CFtpResponse clsRes;

	return Recv( clsRes, iWantCode );
}
