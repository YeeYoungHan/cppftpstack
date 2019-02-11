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
#include <stdio.h>
#include "TimeString.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ���� �ð��� ��,��,��,��,��,�� ����ü�� �����Ѵ�.
 * @param iTime ���� �ð�
 * @param sttTm ��,��,��,��,��,�� ����ü
 */
void LocalTime( time_t iTime, struct tm & sttTm )
{
#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif
}

/**
 * @ingroup SipPlatform
 * @brief �ð� ������ �ð� ���ڿ� ������ �����Ѵ�.
 * @param iTime			�ð� ����
 * @param pszTime		�ð� ���ڿ��� ����� ����
 * @param iTimeSize pszTime ������ ũ��
 */
void GetDateTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszTime, iTimeSize, "%04d%02d%02d%02d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday
		, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

/**
 * @ingroup SipPlatform
 * @brief ����Ͻú��� ���ڿ��� �����Ѵ�.
 * @param pszTime		����Ͻú��� ���ڿ� ���� ����
 * @param iTimeSize pszTime ������ ũ��
 */
void GetDateTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief ����� ���ڿ��� �����Ѵ�.
 * @param iTime			�ð�
 * @param pszDate		����� ���ڿ��� ���� ����
 * @param iDateSize pszDate ������ ũ��
 */
void GetDateString( time_t iTime, char * pszDate, int iDateSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszDate, iDateSize, "%04d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday );
}

/**
 * @ingroup SipPlatform
 * @brief ����� ���ڿ��� �����Ѵ�.
 * @param pszDate		����� ���ڿ��� ���� ����
 * @param iDateSize pszDate ������ ũ��
 */
void GetDateString( char * pszDate, int iDateSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateString( iTime, pszDate, iDateSize );
}

/**
 * @ingroup SipPlatform
 * @brief �ú��� ���ڿ��� �����Ѵ�.
 * @param iTime			�ð�
 * @param pszTime		�ú��� ���ڿ� ���� ����
 * @param iTimeSize pszTime ������ ũ��
 */
void GetTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszTime, iTimeSize, "%02d%02d%02d", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

/**
 * @ingroup SipPlatform
 * @brief �ú��� ���ڿ��� �����Ѵ�.
 * @param pszTime		�ú��� ���ڿ� ���� ����
 * @param iTimeSize pszTime ������ ũ��
 */
void GetTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief ����Ͻú��� ���ڿ��� time_t �� ��ȯ�Ѵ�.
 * @param pszTime ����Ͻú��� ���ڿ�
 * @returns �����ϸ� time_t ���� ���ϵǰ� �����ϸ� 0 �� ���ϵȴ�.
 */
time_t ParseDateTimeString( const char * pszTime )
{
	struct tm	sttTm;
	int iLen = (int)strlen( pszTime );

	if( iLen < 14 ) return 0;

	memset( &sttTm, 0, sizeof(sttTm) );

	sttTm.tm_year = GetInt( pszTime, 4 ) - 1900;
	sttTm.tm_mon = GetInt( pszTime+4, 2 ) - 1;
	sttTm.tm_mday = GetInt( pszTime+6, 2 );
	sttTm.tm_hour = GetInt( pszTime+8, 2 );
	sttTm.tm_min = GetInt( pszTime+10, 2 );
	sttTm.tm_sec = GetInt( pszTime+12, 2 );

	return mktime( &sttTm );
}
