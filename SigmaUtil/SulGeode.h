// SulGeode.h

#ifndef __SULGEODE_H__
#define __SULGEODE_H__

#include "SulExport.h"
#include <osg/geode>

class SUL_EXPORT CSulGeode : public osg::Geode
{
public:
	CSulGeode();
	CSulGeode( osg::Drawable* geom );

	void enableBlend();
	void zbufferWrite( bool bEnable );
	void enableDebugTexture();
	void lighting( bool bEnable );
};


#endif // __SULGEODE_H__