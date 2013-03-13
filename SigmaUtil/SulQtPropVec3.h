// SulQtPropVec3.h

#ifndef __SULQTPROPVEC3_H__
#define __SULQTPROPVEC3_H__

#include "SulQtPropString.h"

class CSulQtPropVec3 : public CSulQtPropString
{
public:
	CSulQtPropVec3( const CSulString& label, const osg::Vec3& v=osg::Vec3(0,0,0) ) :
		CSulQtPropString( label, CSulString( v ) )
		{
		}
};

#endif // __SULQTPROPVEC3_H__