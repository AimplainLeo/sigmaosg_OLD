// SulPaths.h

#ifndef __SULPATHS_H__
#define __SULPATHS_H__

#include "SulExport.h"
#include "SulString.h"
#include <osg/Referenced>
#include <vector>

class SUL_EXPORT CSulPaths : public osg::Referenced
{
public:
	typedef std::vector<CSulString>	VECTOR_PATH;

public:
	void				Add( const CSulString& sPath );
	sigma::uint32		GetCount();
	const VECTOR_PATH&	GetPaths() const;

	void				Load( class CSulXmlAttr* pAttr );
	void				Save( class CSulXmlWriter* pWriterXML );

private:
	VECTOR_PATH			m_vecPath;
};

#endif // __SULPATHS_H__