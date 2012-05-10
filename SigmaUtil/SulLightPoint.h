// SulLightPoint.h

#ifndef __SULLIGHTPOINT_H__
#define __SULLIGHTPOINT_H__

#include "SulLightBase.h"
#include "SulExport.h"

class SUL_EXPORT CSulLightPoint : public CSulLightBase
{
public:
			CSulLightPoint( CSulLightManager* lm );

	void	traverse( osg::NodeVisitor& nv );
};

typedef std::vector< CSulLightPoint* >	VEC_LIGHT_POINT;


#endif // __SULLIGHTPOINT_H__