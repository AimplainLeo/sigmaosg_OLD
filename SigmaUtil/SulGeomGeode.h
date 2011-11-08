// SulGeomGeode.h

#ifndef __SULGEOMGEODE_H__
#define __SULGEOMGEODE_H__

#include "SulExport.h"
#include <osg/geode>

class SUL_EXPORT CSulGeomGeode : public osg::Geode
{
public:
	void enableBlend();
	void zbuffer( bool bEnable );
	void enableDebugTexture();
};

#endif // __SULGEOMGEODE_H__