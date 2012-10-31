// SulLightBase.cpp

#include "stdafx.h"
#include "SulLightBase.h"
#include "SulLightManager.h"
#include <limits>

CSulLightBase::CSulLightBase( CSulLightManager* lm )
{
	m_lm = lm;
	m_dirty = true;
	m_enabled = true;
	m_ambientColor.set( 0, 0, 0, 0 );
	dirtyBound();
}

void CSulLightBase::setDiffuseColor( const osg::Vec4& color )
{
	m_diffuseColor = color;
	m_dirty = true;
}

const osg::Vec4& CSulLightBase::getDiffuseColor() const
{
	return m_diffuseColor;
}

void CSulLightBase::setAmbientColor( const osg::Vec4& color )
{
	m_ambientColor = color;
	m_dirty = true;
}

const osg::Vec4& CSulLightBase::getAmbientColor() const
{
	return m_ambientColor;
}

void CSulLightBase::setId( sigma::int32 id )
{
	m_id = id;
}

sigma::int32 CSulLightBase::getId()
{
	return m_id;
}

void CSulLightBase::setDirtyFlag( bool dirty )
{
	m_dirty = dirty;
}

bool CSulLightBase::isDirty() const
{
	return m_dirty;
}

void CSulLightBase::setEnabled( bool enabled )
{
	m_enabled = enabled;
	m_dirty = true;
}

bool CSulLightBase::isEnabled() const
{
	return m_enabled;
}

void CSulLightBase::setPosition( const osg::Vec3& pos )
{
	m_pos = pos;
	m_dirty = true;
	dirtyBound();
}

const osg::Vec3& CSulLightBase::getPosition() const
{
	return m_pos;
}

void CSulLightBase::setViewPosition( const osg::Vec3& pos )
{
	m_posViewMatrix = pos;
	m_dirty = true;
}

const osg::Vec3& CSulLightBase::getViewPosition()
{
	return m_posViewMatrix;
}

void CSulLightBase::setConstantAtt( float att )
{
	m_attConstant = att;
	m_dirty = true;
	dirtyBound();
}

void CSulLightBase::setLinearAtt( float att )
{
	m_attLinear = att;
	m_dirty = true;
	dirtyBound();
}

void CSulLightBase::setQuadraticAtt( float att )
{
	m_attQuadratic = att;
	m_dirty = true;
	dirtyBound();
}

float CSulLightBase::getConstantAtt() const
{
	return m_attConstant;
}

float CSulLightBase::getLinearAtt() const
{
	return m_attLinear;
}

float CSulLightBase::getQuadraticAtt() const
{
	return m_attQuadratic;
}

osg::BoundingSphere CSulLightBase::computeBound() const
{
	//BoundingSphere bsphere(Group::computeBound());
	// http://www.wolframalpha.com/input/?i=solve+a%3D1%2F%28C%2BL*d%2BQ*d%5E2%29+for+d

	float a = 0.0001f;
	float C = m_attConstant;
	float L = m_attLinear;
	float Q = m_attQuadratic;
	float d;

	if ( Q!=0.0f )
	{
		d = ( sqrt( -a*(4.0f*a*C*Q-a*L*L-4*Q) ) - a*L ) / (2.0f*a*Q);
	}
	else if ( Q==0.0f && L!=0.0f )
	{
		d = (1.0f-a*C)/(a*L);
	}
	else
	{
		d = std::numeric_limits<float>::max();
	}

	//d = 10000000.0f;
	//d = 1.0f;
	osg::BoundingSphere bs( m_pos, d );
	return bs;
}

void CSulLightBase::traverse( osg::NodeVisitor& nv )
{
	osg::Node::traverse( nv );

	if( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		// if we get to here.. then it is because the light is visible in the scene for this camera

		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);

		osg::RefMatrix* pMV = cv->getModelViewMatrix();
		setViewPosition( getPosition() * (*pMV) );
	}
}