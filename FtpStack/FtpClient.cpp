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
#include "FileUtility.h"
#include "StringUtility.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>

#ifdef WIN32

#include <io.h>
#define OPEN_READ_FLAG	( _O_RDONLY | _O_BINARY )
#define OPEN_WRITE_FLAG	( _O_WRONLY | _O_CREAT | _O_BINARY | _O_TRUNC )
#define OPEN_READ_MODE	_S_IREAD
#define OPEN_WRITE_MODE	( _S_IREAD | _S_IWRITE )

#define open		_open
#define read		_read
#define write		_write
#define close		_close
#define unlink	DeleteFile

#else

#include <unistd.h>
#define OPEN_READ_FLAG	O_RDONLY
#define OPEN_WRITE_FLAG	( O_WRONLY | O_CREAT | O_TRUNC )
#define OPEN_READ_MODE	( S_IRUSR | S_IRGRP | S_IROTH )
#define OPEN_WRITE_MODE ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

#endif

#include "FtpClientDirectory.hpp"
#include "FtpClientFile.hpp"
#include "FtpClientList.hpp"

CFtpClient::CFtpClient() : m_hSocket(INVALID_SOCKET), m_iServerPort(21), m_iTimeout(10), m_bUseUtf8(false)
{
}

CFtpClient::~CFtpClient()
{
	Close();
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버에 연결한다.
 * @param pszServerIp FTP 서버 IP 주소
 * @param iServerPort FTP 서버 포트 번호
 * @param bUseUtf8		파일 이름을 UTF8 로 변환하는가?
 * @returns FTP 서버 연결에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Connect( const char * pszServerIp, int iServerPort, bool bUseUtf8 )
{
	if( m_hSocket != INVALID_SOCKET ) return false;

	m_hSocket = TcpConnect( pszServerIp, iServerPort );
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s TcpConnect(%s:%d) error(%d)", __FUNCTION__, pszServerIp, iServerPort, GetError() );
		return false;
	}

	m_strServerIp = pszServerIp;
	m_iServerPort = iServerPort;
	m_bUseUtf8 = bUseUtf8;

	if( Recv( 220 ) == false )
	{
		Close();
		return false;
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버 연결을 종료한다.
 */
void CFtpClient::Close()
{
	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버에 로그인한다.
 * @param pszUserId			아이디
 * @param pszPassWord		비밀번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Login( const char * pszUserId, const char * pszPassWord )
{
	if( Send( "USER %s", pszUserId ) == false || 
			Recv( 331 ) == false || 
			Send( "PASS %s", pszPassWord ) == false ||
			Recv( 230 ) == false )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버로 binary type 및 passive mode 명령을 전송한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::SendBinaryPassive( )
{
	if( Send( "TYPE I" ) == false ||
			Recv( 200 ) == false )
	{
		return false;
	}

	CFtpResponse clsRes;

	if( Send( "PASV" ) == false ||
			Recv( clsRes, 227 ) == false )
	{
		return false;
	}

	if( clsRes.GetIpPort( m_strDataIp, m_iDataPort ) == false )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버로 명령을 전송한다.
 * @param fmt			명령 
 * @param	...			명령 인자
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Send( const char * fmt, ... )
{
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s not connected", __FUNCTION__ );
		return false;
	}

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
		Close();
		return false;
	}

	CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%.*s]", m_strServerIp.c_str(), m_iServerPort, iSendLen, szSendBuf );

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버의 응답을 수신한다.
 * @param clsResponse 응답 메시지를 파싱하는 객체
 * @param iWantCode		원하는 응답 코드
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Recv( CFtpResponse & clsResponse, int iWantCode )
{
	if( m_hSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "%s not connected", __FUNCTION__ );
		return false;
	}

	char	szRecvBuf[8192];
	int		n, iPos;

	if( m_strRecvBuf.empty() == false )
	{
		iPos = clsResponse.Parse( m_strRecvBuf.c_str(), (int)m_strRecvBuf.length() );
		if( iPos > 0 )
		{
			m_strRecvBuf.erase( 0, iPos );
			goto CHECK_WANT_CODE;
		}
	}

	while( 1 )
	{
		n = TcpRecv( m_hSocket, szRecvBuf, sizeof(szRecvBuf), m_iTimeout );
		if( n <= 0 )
		{
			CLog::Print( LOG_ERROR, "%s TcpRecv(%s) error(%d)", __FUNCTION__, m_strRecvBuf.c_str(), GetError() );
			Close();
			return false;
		}

		CLog::Print( LOG_NETWORK, "TcpRecv(%s:%d) [%.*s]", m_strServerIp.c_str(), m_iServerPort, n, szRecvBuf );

		m_strRecvBuf.append( szRecvBuf, n );
		iPos = clsResponse.Parse( m_strRecvBuf.c_str(), (int)m_strRecvBuf.length() );
		if( iPos > 0 )
		{
			m_strRecvBuf.erase( 0, iPos );

			if( clsResponse.m_iCode != 0 ) break;
		}
	}

CHECK_WANT_CODE:
	if( iWantCode )
	{
		if( iWantCode == 150 )
		{
			if( clsResponse.m_iCode != 150 && clsResponse.m_iCode != 125 )
			{
				CLog::Print( LOG_ERROR, "%s reply code(%d) != want code(%d)", __FUNCTION__, clsResponse.m_iCode, iWantCode );
				return false;
			}
		}
		else if( clsResponse.m_iCode != iWantCode )
		{
			CLog::Print( LOG_ERROR, "%s reply code(%d) != want code(%d)", __FUNCTION__, clsResponse.m_iCode, iWantCode );
			return false;
		}
	}

	return true;
}

/**
 * @ingroup FtpStack
 * @brief FTP 서버의 응답을 수신한다.
 * @param iWantCode 원하는 응답 코드
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CFtpClient::Recv( int iWantCode )
{
	CFtpResponse clsRes;

	return Recv( clsRes, iWantCode );
}
