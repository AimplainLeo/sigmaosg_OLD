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
					CSulString( float f, sigma::uint32 decimalCount=0 );

	// modifiers
	void			Format( const char* Format, ... );
	void			MakeLower();
	void			Trim();
	void			replaceAll( const std::string& searchFor, const std::string& changeTo );

	// conversions
	float			asFloat() const;
	double			asDouble() const;
	sigma::uint32	asUint32() const;
	sigma::int32	asInt32() const;
	sigma::int64	asInt64() const;
	bool			asBool() const;
	osg::Vec4		asVec4() const;

	CSulString		char2hex( char dec ) const;

	// web
	void			urlSetHost( const CSulString& host );
	CSulString		urlGetHost();
	CSulString		urlGetParameterStringOnly();
	CSulString		urlGetParameter( const CSulString& name );
	void			urlAddParameter( const CSulString& name, const CSulString& value );
	CSulString		urlGetDecode() const;
	CSulString		urlGetEncode() const;

	CSulString		GetStartWord( char c ) ;
	
};

#endif // __SULSTRING_H__