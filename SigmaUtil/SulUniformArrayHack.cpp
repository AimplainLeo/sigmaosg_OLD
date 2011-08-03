// SulUniformArrayHack.cpp

#include "stdafx.h"
#include "SulUniformArrayHack.h"

CSulUniformArrayHack::CSulUniformArrayHack( const CSulString& name, sigma::int32 numElements, osg::Uniform::Type type, osg::StateSet* ssToReceive )
{
	m_u1 = new osg::Uniform( type,	name, numElements );
	m_u2 = new osg::Uniform( type,	name+"[0]", numElements );

	ssToReceive->addUniform( m_u1 );
	ssToReceive->addUniform( m_u2 );
}

void CSulUniformArrayHack::setElement( sigma::int32 index, const osg::Vec3& value )
{
	m_u1->setElement( index, value );
	m_u2->setElement( index, value );
}

void CSulUniformArrayHack::setElement( sigma::int32 index, const osg::Vec4& value )
{
	m_u1->setElement( index, value );
	m_u2->setElement( index, value );
}

void CSulUniformArrayHack::setElement( sigma::int32 index, float value )
{
	m_u1->setElement( index, value );
	m_u2->setElement( index, value );
}

void CSulUniformArrayHack::setElement( sigma::int32 index, sigma::int32 value )
{
	m_u1->setElement( index, value );
	m_u2->setElement( index, value );
}

void CSulUniformArrayHack::setElement( sigma::int32 index, bool value )
{
	m_u1->setElement( index, value );
	m_u2->setElement( index, value );
}
