#ifndef __AddGeometryFromPrototypeFunctor_H__
#define __AddGeometryFromPrototypeFunctor_H__

#include <functional>
#include <osg/Vec3>
#include <osg/Geometry>
#include <osg/ref_ptr>

class AddGeometryFromPrototypeFunctor : public std::unary_function<const osg::Vec3&, void>
{
public:
	AddGeometryFromPrototypeFunctor(osg::Geometry* prototype);

	osg::Geometry* getGeometry() const;

	void operator()(const osg::Vec3& p);

private:
	osg::ref_ptr<osg::Geometry> _geometryPrototype;
	osg::ref_ptr<osg::Vec3Array> _vertexArray;
	osg::Geometry::PrimitiveSetList _primitiveSetList;
};

#endif // __AddGeometryFromPrototypeFunctor_H__