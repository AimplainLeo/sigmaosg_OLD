// SulGeomCrosshairs.h

#ifndef __SULGEOMCROSSHAIRS_H__
#define __SULGEOMCROSSHAIRS_H__

#include "SulTypes.h"
#include "SulExport.h"
#include "SulGeode.h"

class SUL_EXPORT CSulGeodeCrosshairs : public CSulGeode
{
public:
	enum ETYPE
	{
		TYPE_M99			// M/99 HALEM 2S
	};

public:
				CSulGeodeCrosshairs( ETYPE type, float len, const osg::Vec4& color=osg::Vec4(0,0,0,1) );

	void		initConstructor();

private:
	void		createTextNumber( float x, float y, sigma::int32 numText );

private:
	ETYPE		m_type;
	float		m_scale;
	osg::Vec4	m_color;
};

#endif // __SULGEOMCROSSHAIRS_H__