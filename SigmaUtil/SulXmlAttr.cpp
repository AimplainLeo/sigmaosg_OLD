// SulXmlAttr.cpp

#include "stdafx.h"
#include "SulXmlAttr.h"

CSulXmlAttr::CSulXmlAttr()
{
	m_dwCount = 0;
	m_strEmpty = "";		// must always be empty
}

CSulXmlAttr::~CSulXmlAttr()
{
	free();
}

// copy constructor
CSulXmlAttr::CSulXmlAttr( const CSulXmlAttr& other )
{
	VECTOR_ATTR_PTR::const_iterator i = other.m_rVectorAttrPtr.begin();
	VECTOR_ATTR_PTR::const_iterator ie = other.m_rVectorAttrPtr.end();
	while( i!=ie )
	{
		SINFO* info = new SINFO;
		info->strName = (*i)->strName;
		info->strValue = (*i)->strValue;
		m_rVectorAttrPtr.push_back( info );
		++i;
	}
}

CSulXmlAttr& CSulXmlAttr::operator=( const CSulXmlAttr& other )
{
	VECTOR_ATTR_PTR::const_iterator i = other.m_rVectorAttrPtr.begin();
	VECTOR_ATTR_PTR::const_iterator ie = other.m_rVectorAttrPtr.end();
	while( i!=ie )
	{
		SINFO* info = new SINFO;
		info->strName = (*i)->strName;
		info->strValue = (*i)->strValue;
		m_rVectorAttrPtr.push_back( info );
		++i;
	}

	return *this;
}

void CSulXmlAttr::free()
{
	VECTOR_ATTR_PTR::iterator iAttr;
	
	iAttr = m_rVectorAttrPtr.begin();
	while ( iAttr!=m_rVectorAttrPtr.end() )
	{
		delete (*iAttr);
		++iAttr;
	}
}

void CSulXmlAttr::reset()
{
	m_dwCount = 0;
}

sigma::uint32 CSulXmlAttr::getCount()
{
	return m_dwCount;
}

CSulXmlAttr::SINFO* CSulXmlAttr::getInfo()
{
	if ( m_dwCount<m_rVectorAttrPtr.size() )
	{
		++m_dwCount;
		return m_rVectorAttrPtr[m_dwCount-1];
	}

	++m_dwCount;
	SINFO* p = new SINFO;
	m_rVectorAttrPtr.push_back( p );
	return p;
}

void CSulXmlAttr::add( const CSulString& sName, sigma::int32 lValue )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue.Format( "%d", lValue );
}

void CSulXmlAttr::add( const CSulString& sName, sigma::uint32 dwValue )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue.Format( "%d", dwValue );
}

void CSulXmlAttr::add( const CSulString& sName, const char* pszAttr )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue = pszAttr;
}

void CSulXmlAttr::add( const CSulString& sName, float fValue )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue.Format( "%f", fValue );
}

void CSulXmlAttr::add( const CSulString& sName, bool bVal )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue.Format( "%s", bVal?"true":"false" );
}

void CSulXmlAttr::add( const CSulString& sName, const osg::Vec4& value )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue = CSulString( value );
}

void CSulXmlAttr::add( const CSulString& sName, const CSulString& value )
{
	SINFO* p = getInfo();
	p->strName = sName;
	p->strValue = value;
}

float CSulXmlAttr::getFloat( const CSulString& sName, float defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asFloat();

	return defaultValue;
}

sigma::uint32 CSulXmlAttr::getUint32( const CSulString& sName, sigma::uint32 defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asUint32();

	return defaultValue;
}

sigma::int32 CSulXmlAttr::getInt32( const CSulString& sName, sigma::int32 defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asInt32();

	return defaultValue;
}

bool CSulXmlAttr::getBool( const CSulString& sName, bool defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asBool();

	return defaultValue;
}

CSulString CSulXmlAttr::getString( const CSulString& sName, CSulString defaultValue )
{
	if ( exist( sName ) )
		return get( sName );

	return defaultValue;
}

osg::Vec3 CSulXmlAttr::getVec3( const CSulString& sName, osg::Vec3 defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asVec3();

	return defaultValue;
}

osg::Vec4 CSulXmlAttr::getVec4( const CSulString& sName, osg::Vec4 defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asVec4();

	return defaultValue;
}

double CSulXmlAttr::getDouble( const CSulString& sName, double defaultValue )
{
	if ( exist( sName ) )
		return get( sName ).asDouble();

	return defaultValue;
}

CSulString& CSulXmlAttr::get( const CSulString& sName )
{
	VECTOR_ATTR_PTR::iterator iAttr;
	
	iAttr = m_rVectorAttrPtr.begin();
	while ( iAttr!=m_rVectorAttrPtr.end() )
	{
		if ( (*iAttr)->strName==sName )
		{
			return (*iAttr)->strValue;
		}

		++iAttr;
	}

	return m_strEmpty;
}

bool CSulXmlAttr::get( const CSulString& sName, float& f )
{
	if ( !exist( sName ) )
	{
		return false;
	}

	CSulString s = get( sName );

	f = s.asFloat();

	return true;
}

bool CSulXmlAttr::get( const CSulString& sName, sigma::uint32& i )
{
	if ( !exist( sName ) )
	{
		return false;
	}

	CSulString s = get( sName );
	if ( s.size() )
	{
		i = s.asUint32();
		return true;
	}

	return false;
}

bool CSulXmlAttr::get( const CSulString& sName, sigma::int8& c )
{
	if ( !exist( sName ) )
	{
		return false;
	}

	CSulString s = get( sName );
	if ( s.size() )
	{
		c = s[0];
		return true;
	}

	return false;
}

bool CSulXmlAttr::get( const CSulString& sName, CSulString& s )
{
	if ( !exist( sName ) )
	{
		return false;
	}

	s = get( sName );

	return true;
}

bool CSulXmlAttr::get( const CSulString& sName, bool& b )
{
	if ( !exist( sName ) )
	{
		return false;
	}

	CSulString s = get( sName );
	s.makeLower();

	if ( s=="true" )
	{
		b = true;
		return true;
	}

	if ( s=="false" )
	{
		b = false;
		return true;
	}

	if ( s=="1" )
	{
		b = true;
		return true;
	}

	if ( s=="0" )
	{
		b = false;
		return true;
	}

	return false;
}

bool CSulXmlAttr::exist( const CSulString& sName )
{
	VECTOR_ATTR_PTR::iterator iAttr;
	
	iAttr = m_rVectorAttrPtr.begin();
	while ( iAttr!=m_rVectorAttrPtr.end() )
	{
		if ( (*iAttr)->strName==sName )
		{
			return true;
		}

		++iAttr;
	}

	return false;
}
