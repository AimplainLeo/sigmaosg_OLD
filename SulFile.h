// SulFile.h

#ifndef __SULFILE_H__
#define __SULFILE_H__

#include "SulExport.h"
#include "SulString.h"
#include "SulTypes.h"
#include "SulPaths.h"
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <vector>

class SUL_EXPORT CSulFile : public osg::Referenced
{
public:
	enum EMODE
	{
		MODE_WRITE,
		MODE_READ_BINARY
	};

	typedef std::vector< std::string >	VEC_STRING;

public:
					CSulFile();
					CSulFile( CSulPaths* sPaths );

	bool			Open( class CSulString sFile, EMODE eMode=MODE_READ_BINARY );
	bool			Close();
	bool			Print( const CSulString& s );
	
	bool 			Save( class CSulString& sFile, sigma::int8* pBufData, sigma::uint32 dwBufSize );
	sigma::uint8* 	Load( const class CSulString& sFile );
	
	sigma::uint32 	GetSize();
	sigma::uint8*	GetDataPtr();
	
	VEC_STRING		getTextLines( sigma::uint32 maxChars=1024 );

protected:
					~CSulFile();

private:
	sigma::uint8*	m_pBuf;
	sigma::uint32	m_dwBufSize;

//	FILE*			m_pFile;

	osg::ref_ptr<CSulPaths>	m_rPaths;

	// vs2003
#if !defined(WIN32) || (_MSC_VER>=1310 && _MSC_VER<1400)
	FILE*			m_pFile;
#else
	FILE*			m_pFile;
#endif



};

#endif // __SULFILE_H__
