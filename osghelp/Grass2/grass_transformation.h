// grass_transformation.h

#ifndef __GRASS_TRANSFORMATION_H__
#define __GRASS_TRANSFORMATION_H__

#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>

class CGrassTransformation : public osg::PositionAttitudeTransform
{
public:
	CGrassTransformation( osgViewer::Viewer* pViewer );
};

#endif //__GRASS_TRANSFORMATION_H__