#ifndef __GeometryInstancingBuilder_H__
#define __GeometryInstancingBuilder_H__


#include <vector>
#include <osg/Vec3>
#include <osg/Geometry>
#include <osg/Node>

class GeometryInstancingBuilder
{
public:
	void setPositionList(const std::vector<osg::Vec3>& positionList) { _positionList = positionList; }
	void setInstanceGeometry(osg::Geometry* geometry) { _geometryPrototype = geometry; }
	osg::Node* build();

private:
	std::vector<osg::Vec3> _positionList;
	osg::ref_ptr<osg::Geometry> _geometryPrototype;
};

#endif // __GeometryInstancingBuilder_H__
