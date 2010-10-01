// GeometryInstancingBuilder.cpp

#include "stdafx.h"
#include "GeometryInstancingBuilder.h"
#include "AddGeometryFromPrototypeFunctor.h"
#include <algorithm>
#include <osg/Geode>

osg::Node* GeometryInstancingBuilder::build()
{
	AddGeometryFromPrototypeFunctor functor = std::for_each(_positionList.begin(), _positionList.end(),
		AddGeometryFromPrototypeFunctor(_geometryPrototype));

	osg::Geode* geode = new osg::Geode;
	geode->addDrawable(functor.getGeometry());
	return geode;
}