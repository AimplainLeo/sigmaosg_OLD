// SulStringParam.h

#ifndef __SULSTRINGPARAM_H__
#define __SULSTRINGPARAM_H__

#include "SulString.h"
#include "SulExport.h"

class SUL_EXPORT CSulStringParam
{
private:
	typedef	std::multimap< CSulString, CSulString > MULTIMAP_PARAMVALUE;

public:
						CSulStringParam( const CSulString& s );

	void				addParamString( const CSulString& s );
	void				add( const CSulString& sParam, const CSulString& sValue );
	bool				get( const CSulString& sParam, float& f, sigma::int32 index=0 );
	bool				get( const CSulString& sParam, CSulString& sOut, sigma::int32 index=0 );
	bool				get( const CSulString& sParam, bool& b, sigma::int32 index=0 );
	bool				get( const CSulString& sParam, sigma::uint32& i, sigma::int32 index=0 );

private:
	MULTIMAP_PARAMVALUE	m_multimapParamValue;
};

#endif // __SULSTRINGPARAM_H__