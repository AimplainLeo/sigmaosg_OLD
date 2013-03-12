// SulString.h

#ifndef __SULSTRING_H__
#define __SULSTRING_H__

#include "SulExport.h"
#include "SulTypes.h"
#include <string>

class SUL_EXPORT CSulString : public std::string
{
public:
	typedef std::vector<CSulString> VEC_STRING;

public:
					CSulString();
					CSulString( const sigma::int8* pszText );
					CSulString( const CSulString& s );
					CSulString( const std::string& s );
					CSulString( sigma::uint8 c );
					CSulString( sigma::uint32 i );
					CSulString( sigma::int32 i );
					CSulString( sigma::int64 c );
					CSulString( float f, sigma::uint32 decimalCount=10 );
					CSulString( double d, sigma::uint32 decimalCount=10 );
					CSulString( const osg::Vec3 v, sigma::uint32 decimalCount=10 );
					CSulString( const osg::Vec4 v, sigma::uint32 decimalCount=10 );
					CSulString( const osg::Vec4s v );

	// modifiers
	void			Format( const char* Format, ... );
	void			makeLower();
	void			Trim();
	void			replaceAll( const std::string& searchFor, const std::string& changeTo );

	// conversions
	float			asFloat() const;
	double			asDouble() const;
	sigma::uint32	asUint32() const;
	sigma::int32	asInt32() const;
	sigma::int64	asInt64() const;
	bool			asBool() const;
	osg::Vec3		asVec3() const;
	osg::Vec4		asVec4() const;
	osg::Vec4s		asVec4s() const;

	CSulString		char2hex( char dec ) const;

	// web
	void			urlSetHost( const CSulString& host );
	CSulString		urlGetHost();
	CSulString		urlGetParameterStringOnly();
	CSulString		urlGetParameter( const CSulString& name );
	void			urlAddParameter( const CSulString& name, const CSulString& value );
	CSulString		urlGetDecode() const;
	CSulString		urlGetEncode() const;

	// find first string until character match (FIXME: method should be renamed to better describe what it is doing)
	CSulString		GetStartWord( char c ) ;

	// returns extension afer the period at the end of the string (ex. "myfile.exe" would return "exe")
	CSulString		getExtension();
	
};

#endif // __SULSTRING_H__