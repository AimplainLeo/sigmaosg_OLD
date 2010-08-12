// SulStringList.h

#ifndef __SULSTRINGLIST_H__
#define __SULSTRINGLIST_H__

#include "SulExport.h"
#include "SulTypes.h"
#include "SulString.h"

class SUL_EXPORT CSulStringList : public osg::Referenced
{
public:
	typedef std::vector<std::string>	VECTOR_STRING;

public:
							CSulStringList( const class CSulString& s );

	void					Tokenize( const CSulString& s );

	const VECTOR_STRING&	GetList() const;

	CSulString				getString( sigma::uint32 index );

	sigma::uint32			getIndex( const CSulString& s );

private:
	VECTOR_STRING			m_vecString;
};

#endif // __SULSTRINGLIST_H__