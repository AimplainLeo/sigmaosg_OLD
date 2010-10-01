// AddGeometryFromPrototypeFunctor.cpp

#include "stdafx.h"
#include "AddGeometryFromPrototypeFunctor.h"

AddGeometryFromPrototypeFunctor::AddGeometryFromPrototypeFunctor(osg::Geometry* prototype) :
_geometryPrototype(prototype)
{
	_vertexArray = new osg::Vec3Array;
}

void AddGeometryFromPrototypeFunctor::operator()(const osg::Vec3& p)
{
	unsigned int _primitiveIndexOffset = _vertexArray->size();
	// copy vertices from prototype with offset
	const osg::Vec3Array* prototypeVertexArray = dynamic_cast<const osg::Vec3Array*>(_geometryPrototype->getVertexArray());
	if(prototypeVertexArray)
	{
		for(osg::Vec3Array::const_iterator it = prototypeVertexArray->begin(); it != prototypeVertexArray->end(); ++it)
		{
			_vertexArray->push_back(*it + p);
		}
	}

	// copy primitive sets with index offset
	const osg::Geometry::PrimitiveSetList& prototypePrimitiveList = _geometryPrototype->getPrimitiveSetList();
	for(osg::Geometry::PrimitiveSetList::const_iterator it = prototypePrimitiveList.begin(); it != prototypePrimitiveList.end(); ++it)
	{
		osg::PrimitiveSet* clone = dynamic_cast<osg::PrimitiveSet*>((*it)->clone(osg::CopyOp::SHALLOW_COPY));
		clone->offsetIndices(_primitiveIndexOffset);
	    _primitiveSetList.push_back(clone);
	}
}

osg::Geometry* AddGeometryFromPrototypeFunctor::getGeometry() const
{
	osg::Geometry* result = new osg::Geometry;
	result->setVertexArray(_vertexArray);
	result->setPrimitiveSetList(_primitiveSetList);
	return result;
}