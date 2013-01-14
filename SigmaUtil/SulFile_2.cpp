// SulFile_2.cpp

#include "stdafx.h"
#include "SulFile.h"
#include <osgDB/FileUtils>

//////////////////////////////////////////////////////////////////////////////////////
//
// NOTE:
//	This version uses the new microsoft _s methods
//
//	#if _MSC_VER >= 1400
//		// this is Visual C++ 2005
//	#elif _MSC_VER >= 1310
//		// this is Visual c++ .NET 2003
//	#elif _MSC_VER > 1300
//		// this is Visual C++ .NET 2002
//	#endif
//
//////////////////////////////////////////////////////////////////////////////////////

// if win32 and not vs2003
#if defined(WIN32) && !(_MSC_VER>=1310 && _MSC_VER<1400)

bool CSulFile::Open( CSulString sFile, EMODE eMode )
{
	CSulString fileFound = osgDB::findDataFile( sFile );
	if ( fileFound.empty() )
		return false;

	CSulString	sMode;
	switch ( eMode )
	{
		case MODE_READ_BINARY:	sMode="rb";	break;
		case MODE_WRITE:		sMode="w";	break;

		default:
			assert(0);	// missing implementaiton
	}

	errno_t			err;

	err = fopen_s( &m_pFile, fileFound.c_str(), sMode.c_str() );
	if ( err )
	{
		return false;
	}

	return true;
}

bool CSulFile::Close()
{
	fclose( m_pFile );
	return true;
}

bool CSulFile::Print( const CSulString& s )
{
	sigma::int32 iRet = fprintf_s( m_pFile, s.c_str() );
	return iRet?true:false;
}

bool CSulFile::Save( CSulString& sFile, char* pBufData, sigma::uint32 dwBufSize )
{
	FILE*			pFile;
	errno_t			err;

	// open file for streaming
	err = fopen_s( &pFile, sFile.c_str(), "wb" );
	if ( err )
	{
		return false;
	}

	if ( !fwrite( pBufData, dwBufSize, 1, pFile  ) )
	{
		fclose( pFile );
		return false;
	}
		
	// close file
	fclose( pFile );

	return true;
}

sigma::uint8* CSulFile::Load( const CSulString& sFile )
{
	FILE*			pFile = 0;
	errno_t			err;

	// delete anything we had before
	if ( m_pBuf )
	{
		delete[] m_pBuf;
		m_pBuf = 0;
	}

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
			
			err = fopen_s( &pFile, s.c_str(), "rb" );
			if ( !err )
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
		CSulString fileFound = osgDB::findDataFile( sFile );
		if ( fileFound.empty() )
			return false;

		err = fopen_s( &pFile, fileFound.c_str(), "rb" );
		if ( err )
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

CSulFile::VEC_STRING CSulFile::getTextLines( sigma::uint32 maxChars )
{
	char* line = new char[maxChars];
	VEC_STRING vs;

	while ( fgets( line, maxChars-1, m_pFile ) )
	{
		vs.push_back( line );
	}

	delete line;

	return vs;
}

#endif // TARGET_WINDOWS
