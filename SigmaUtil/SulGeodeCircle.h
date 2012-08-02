// SulGeodeCircle.h

#ifndef __SULGEODECIRCLE_H__
#define __SULGEODECIRCLE_H__

#include "SulGeomCircle.h"
#include "SulGeode.h"
#include "SulExport.h"

class SUL_EXPORT CSulGeodeCircle : public CSulGeode
{
public:
					CSulGeodeCircle( float radius );

	void			setOffset( const osg::Vec3& ofs );

private:
	void			initConstructor();

private:
	float							m_radius;
	osg::ref_ptr<CSulGeomCircle>	m_geomCircle;
};

#endif // __SULGEODECIRCLE_H__