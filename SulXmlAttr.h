// SulXmlAttr.h

#ifndef __SULXMLATTR_H__
#define __SULXMLATTR_H__

#include "SulExport.h"
#include "SulString.h"
#include <osg/Referenced>
#include <map>
#include <string>
#include <vector>

class SUL_EXPORT CSulXmlAttr : public osg::Referenced
{
public:
	struct SINFO
	{
		CSulString	strName;
		CSulString	strValue;
	};

	typedef std::vector<SINFO*> VECTOR_ATTR_PTR;

public:
					CSulXmlAttr();
					~CSulXmlAttr();

	void			free();
	void			reset();

	sigma::uint32	getCount();

	void			add( const CSulString& sName, bool bVal );
	void			add( const CSulString& sName, const char* pszAttr );
	void			add( const CSulString& sName, sigma::int32 dwValue );
	void			add( const CSulString& sName, sigma::uint32 dwValue );
	void			add( const CSulString& sName, float fValue );

	CSulString&		get( const CSulString& sName );
	bool			get( const CSulString& sName, float& f );
	bool			get( const CSulString& sName, bool& b );
	bool			get( const CSulString& sName, sigma::int8& c );
	bool			get( const CSulString& sName, sigma::uint32& i );
	bool			get( const CSulString& sName, CSulString& s );

	bool			exist( const CSulString& sName );

private:
	SINFO*			getInfo();	

private:
	CSulString		m_strName;			// tag name
	sigma::uint32	m_dwCount;			// number of actual used sinfo
	CSulString		m_strEmpty;

public:
	VECTOR_ATTR_PTR	m_rVectorAttrPtr;	// attributes
};

#endif // __SULXMLATTR_H__
