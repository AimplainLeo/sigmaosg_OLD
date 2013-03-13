// SulQtPropVec4s.h

#ifndef __SULQTPROPVEC4S_H__
#define __SULQTPROPVEC4S_H__

#include "SulQtPropString.h"
#include "SulString.h"

class CSulQtPropVec4s : public CSulQtPropString
{
public:
	CSulQtPropVec4s( const CSulString& label, const osg::Vec4s& v=osg::Vec4s(0,0,0,0) ) :
		CSulQtPropString( label, CSulString( v ) )
		{
		}
};


#endif // __SULQTPROPVEC4S_H__