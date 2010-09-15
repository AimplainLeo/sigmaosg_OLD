// SulString.cpp

#include "stdafx.h"
#include "SulString.h"

CSulString::CSulString() :
std::string()
{
}

CSulString::CSulString( const sigma::int8* pszText ) :
std::string( pszText )
{
}

CSulString::CSulString( const CSulString& s ) :
std::string( s )
{
}

CSulString::CSulString( const std::string& s ) :
std::string( s )
{
}

CSulString::CSulString( sigma::uint8 c )
{
	CSulString s;
	s.Format( "%c", c );
	assign( s.c_str() );
}

CSulString::CSulString( float f )
{
	CSulString s;
	s.Format( "%f", f );
	assign( s.c_str() );
}

void CSulString::Format( const char* Format, ... )
{
#ifdef WIN32
	// FIXME: this is NOT good at all... crash if we exceed!
	char szTmp[32000];

	va_list ArgList;
	va_start( ArgList, Format );
	//vsprintf_s( szTmp, sizeof(szTmp), Format, ArgList );
	VSPRINTF( szTmp, sizeof(szTmp), Format, ArgList );
	va_end( ArgList );
	//SetText( szTmp );

	assign( szTmp );
#else
	assert( 0 ); // FIXME: needs implementation for other platforms
#endif
}

void CSulString::MakeLower()
{
	std::transform( begin(), end(), begin(), tolower );
}

void CSulString::Trim()
{
	//@TKG Test to make sure that we don't have a "" string with only white space
	if (size()== 0)
	{
		return;
	}
	char* ws = " \t\n\r";
	std::string s(*this);
	std::string::size_type i = s.find_first_not_of(ws);
	std::string::size_type j = s.find_last_not_of(ws);
	//@TKG Test to make sure that if we only have white space that we dont do some nasty stuff
	if (i == j)
	{
		return;
	}
	s = s.substr(i, j-i+1);
	assign(s);
}

float CSulString::asFloat()
{
	return (float)atof( c_str() );
}

double CSulString::asDouble()
{
	return (float)strtod( c_str(), 0 );
}

sigma::uint32 CSulString::asUint32()
{
	return (sigma::uint32)atoi( c_str() );
}

sigma::int32 CSulString::asInt32()
{
	return (sigma::int32)atoi( c_str() );
}

CSulString CSulString::GetStartWord( char c ) 
{
	std::string::size_type i = find_first_of( c, 0 )+1;
	return substr( 0, i );
}

bool CSulString::asBool()
{
	if ( empty() )
	{
		return false;
	}

	CSulString tmp = *this;
	tmp.MakeLower();

	if ( tmp=="1" ) return true;
	if ( tmp=="true" ) return true;
	if ( tmp=="yes" ) return true;

	return false;
}

osg::Vec4 CSulString::asVec4()
{
	if ( !empty() )
	{
		float r, g, b, a;
		sscanf( c_str(), "%f,%f,%f,%f", &r, &g, &b, &a ); 
		return osg::Vec4( r, g, b, a );
	}

	return osg::Vec4( 1, 1, 1, 1 );
}