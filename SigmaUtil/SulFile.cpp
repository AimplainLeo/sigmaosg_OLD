// SulFile.cpp

#include "stdafx.h"
#include "SulFile.h"
#include "SulString.h"

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

CSulFile::CSulFile()
{
	m_pBuf 		= 0;
	m_dwBufSize	= 0;
}

CSulFile::CSulFile( CSulPaths* pPaths )
{
	m_pBuf 		= 0;
	m_dwBufSize	= 0;
	m_rPaths = pPaths;
}

CSulFile::~CSulFile()
{
	if ( m_pBuf )
	{
		delete[] m_pBuf;
	}
}

sigma::uint32 CSulFile::GetSize()
{
	return m_dwBufSize;
}

sigma::uint8* CSulFile::GetDataPtr()
{
	return m_pBuf;
}
