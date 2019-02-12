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

#ifndef _FTP_FILE_H_
#define _FTP_FILE_H_

#include <string>
#include <list>

/**
 * @ingroup FtpStack
 * @brief FTP 서버에 존재하는 파일 정보 저장 클래스
 */
class CFtpFile
{
public:
	CFtpFile();

	void Clear();

	std::string m_strFileName;
	bool				m_bFolder;
};

typedef std::list< CFtpFile > FTP_FILE_LIST;

#endif
