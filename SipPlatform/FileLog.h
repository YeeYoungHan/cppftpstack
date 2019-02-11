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

#ifndef _FILE_LOG_H_
#define _FILE_LOG_H_

#include "Log.h"

/**
 * @ingroup SipPlatform
 * @brief ���� �α� ���� Ŭ����
 */
class CFileLog
{
public:
	CFileLog();
	~CFileLog();

public:
	bool Open( const char * pszDirName );
	bool Close( );

	bool Print( EnumLogLevel iLevel, const char * fmt, ... );

	int GetLevel( );
	void SetLevel( int iLevel );
	bool IsPrintLogLevel( EnumLogLevel iLevel );

	void SetMaxLogSize( int iSize );
	void SetMaxFolderSize( int64_t iSize );

private:
	void DeleteOldFile( );

	std::string	m_strDirName;		// �α� ������ ������ ���丮 �̸�
	char				m_szDate[9];		// ���� �������� �α� ���� �̸�
	FILE				* m_sttFd;			// �α� ���� ������

	CSipMutex		m_clsMutex;			// Thread Mutex
	
	int				m_iLevel;					// �α� ���Ͽ� ������ ����
	int				m_iMaxLogSize;		// �ϳ��� �α� ���Ͽ� ������ �� �ִ� �ִ� ũ��
	int				m_iLogSize;				// ������� ����� �α� ũ��
	int				m_iIndex;					// �α� ���� �ε���
	int64_t		m_iMaxFolderSize;	// �α� ������ �ִ� ũ��
	int64_t		m_iFolderSize;		// �α� ���� ũ��
};

#endif
