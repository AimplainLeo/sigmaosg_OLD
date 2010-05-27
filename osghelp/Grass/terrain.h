// terrain.h

#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <osg/node>
#include <osg/referenced>
#include <osg/camera>

class CTerrain : public osg::Referenced
{
public:
	void						Init();
	osg::Node*					getNode();
	osg::Node*					getNodeWithShader();

	// camera to show debug area on terrain
	void						setCam( osg::Camera* pCam );

private:
	osg::ref_ptr<osg::Node>		rTerrain;
	osg::ref_ptr<osg::Group>	rTerrainShader;
};

#endif // __TERRAIN_H__