// SulString.h

#ifndef __SULSTRING_H__
#define __SULSTRING_H__

#include "SulExport.h"
#include "SulTypes.h"
#include <string>

class SUL_EXPORT CSulString : public std::string
{
public:
					CSulString();
					CSulString( const sigma::int8* pszText );
					CSulString( const CSulString& s );
					CSulString( const std::string& s );
					CSulString( sigma::uint8 c );
					CSulString( float f );

	void			Format( const char* Format, ... );
	void			MakeLower();
	void			Trim();

	float			asFloat();
	double			asDouble();
	sigma::uint32	asUint32();
	sigma::int32	asInt32();
	bool			asBool();
	osg::Vec4		asVec4();

	CSulString		GetStartWord( char c ) ;
};

#endif // __SULSTRING_H__