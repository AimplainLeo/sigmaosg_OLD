// SulGeomGeode.h

#ifndef __SULGEOMGEODE_H__
#define __SULGEOMGEODE_H__

#include "SulExport.h"
#include <osg/geode>

class SUL_EXPORT CSulGeomGeode : public osg::Geode
{
public:
	void enableBlend();
	void zbufferWrite( bool bEnable );
	void enableDebugTexture();
	void lighting( bool bEnable );
};

#endif // __SULGEOMGEODE_H__