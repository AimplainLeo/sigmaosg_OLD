// SulFile_1.cpp

#include "stdafx.h"
#include "SulFile.h"
#include "SulPaths.h"

// if not windows or if windows but vs2003 then its ok
#if !defined(WIN32) || (_MSC_VER>=1310 && _MSC_VER<1400)

bool CSulFile::Open( class CSulString sFile, EMODE eMode )
{
	CSulString	sMode;

	switch ( eMode )
	{
		case MODE_READ_BINARY:	sMode="rb";	break;
		case MODE_WRITE:		sMode="w";	break;

		default:
			assert(0);	// missing implementaiton
	}

	m_pFile = fopen( sFile.c_str(), sMode.c_str() );
	if ( !m_pFile )
	{
		return false;
	}

	return true;
}

bool CSulFile::Close()
{
	return fclose( m_pFile )?false:true;
}

bool CSulFile::Print( const CSulString& s )
{
	sigma::int32 iRet = fprintf( m_pFile, s.c_str() );
	return iRet?true:false;
}

bool CSulFile::Save( CSulString& sFile, char* pBufData, sigma::uint32 dwBufSize )
{
	assert( 0 ); // FIXME
	return false;
}

unsigned char* CSulFile::Load( const CSulString& sFile )
{
	FILE*			pFile = 0;
	
	// delete anything we had before
	if ( m_pBuf )
	{
		delete m_pBuf;
		m_pBuf = 0;
	}

	CSulPaths::VECTOR_PATH::iterator i;

	if ( m_rPaths.valid() && m_rPaths->GetCount() )
	{
		const CSulPaths::VECTOR_PATH& vecPath = m_rPaths->GetPaths();
		CSulPaths::VECTOR_PATH::const_iterator i;

		// use paths given
		i = vecPath.begin();
		while ( i!=vecPath.end() )
		{
			CSulString s = (*i);
			s+= sFile;
			
			pFile = fopen( s.c_str(), "rb" );
			if ( pFile )
			{
				break;
			}

			++i;
		}

		if ( i==vecPath.end() )
		{
			return 0;
		}
	}

	if ( !pFile )
	{
		pFile = fopen( sFile.c_str(), "rb" );
		if ( !pFile )
		{
			return 0;
		}
	}

	// get file size
	fseek( pFile, 0, SEEK_END );
	m_dwBufSize = ftell( pFile );
	
	// reset file position
	fseek( pFile, 0, SEEK_SET );

	// alloc memory for buf
	m_pBuf = new unsigned char[m_dwBufSize];
	if ( !m_pBuf )
	{
		fclose( pFile );
		return 0;
	}

	// get file data
	if ( !fread( m_pBuf, m_dwBufSize, 1, pFile ) )
	{
		if ( m_pBuf )
		{
			delete[] m_pBuf;
			m_pBuf = 0;
		}

		fclose( pFile );
		return 0;
	}

	// close file
	fclose( pFile );

	return m_pBuf;
}

#endif 
