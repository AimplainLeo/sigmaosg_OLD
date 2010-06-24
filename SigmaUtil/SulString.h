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
					CSulString( const Sigma::int8* pszText );
					CSulString( const CSulString& s );
					CSulString( const std::string& s );
					CSulString( Sigma::uint8 c );
					CSulString( float f );

	void			Format( const char* Format, ... );
	void			MakeLower();
	void			Trim();

	float			asFloat();
	double			asDouble();
	Sigma::uint32	asUint32();
	Sigma::int32	asInt32();
	bool			asBool();

	CSulString		GetStartWord( char c ) ;
};

#endif // __SULSTRING_H__