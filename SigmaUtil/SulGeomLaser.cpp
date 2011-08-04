// SulGeomLaser.cpp

#include "stdafx.h"
#include "SulGeomLaser.h"
#include <osg/billboard>

osg::Geometry* CSulGeomLaser::createQuad( float radiusStart, float radiusEnd )
{
	// create drawable geometry object
	osg::Geometry* pGeo = new osg::Geometry;

	/*
		ar : 

	   1 ____3____ 5
		|    |    |
		|    |    |
		|    |    |
	    |____|____|
       0     2     4

	*/

	osg::Vec3 ar[12] =
	{
		osg::Vec3( -radiusStart, 0, 0 ),
		osg::Vec3( -radiusEnd,  1, 0 ), 
		osg::Vec3(    0, 0, 0 ),
		osg::Vec3(	 0, 1, 0 ),
		osg::Vec3(  radiusStart,  0, 0 ),
		osg::Vec3(  radiusEnd,  1, 0 )
	};

	osg::Vec3Array* pVerts = new osg::Vec3Array;
	pVerts->push_back( ar[0] );
	pVerts->push_back( ar[1] );
	pVerts->push_back( ar[2] );
	pVerts->push_back( ar[3] );
	pVerts->push_back( ar[4] );
	pVerts->push_back( ar[5] );
	/*
	pVerts->push_back( ar[0] );
	pVerts->push_back( ar[1] );
	pVerts->push_back( ar[2] );
	
	pVerts->push_back( ar[2] );
	pVerts->push_back( ar[1] );
	pVerts->push_back( ar[3] );

	pVerts->push_back( ar[3] );
	pVerts->push_back( ar[5] );
	pVerts->push_back( ar[2] );

	pVerts->push_back( ar[2] );
	pVerts->push_back( ar[5] );
	pVerts->push_back( ar[4] );
	*/
	pGeo->setVertexArray( pVerts );

	// create color array data (each corner of our triangle will have one color component)
    osg::Vec4Array* pColors = new osg::Vec4Array;
    pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 0.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 0.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 0.05f ) );
	pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 0.0f ) );
	pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 0.0f ) );
	pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 0.0f ) );
    pGeo->setColorArray( pColors );

    // make sure that our geometry is using one color per vertex
    pGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

	// create a primitive set (add index numbers)
	osg::DrawElementsUInt* pPrimitiveSet = 
		new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	pPrimitiveSet->push_back( 0 );
	pPrimitiveSet->push_back( 1 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 1 );
	pPrimitiveSet->push_back( 3 );
	pPrimitiveSet->push_back( 3 );
	pPrimitiveSet->push_back( 5 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 2 );
	pPrimitiveSet->push_back( 5 );
	pPrimitiveSet->push_back( 4 );
	pGeo->addPrimitiveSet( pPrimitiveSet );

	osg::StateSet* pStateSet = pGeo->getOrCreateStateSet();
	pStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );
	pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

	return pGeo;
}

void CSulGeomLaser::init()
{
	// this is the group we toggle
	m_rLaserGroup = new osg::Group;
	addChild( m_rLaserGroup );
	

	m_radiusStart = 0.01f;
	m_radiusEnd = 0.01f;
	m_rPat = new osg::PositionAttitudeTransform;
	m_rPat->addChild( createBillboard(m_radiusStart,m_radiusEnd) );
	m_rPat->setDataVariance(osg::Object::DYNAMIC); // children are modified via setRadius
	m_rLaserGroup->addChild( m_rPat );

	// we want to add laser pulsing
	// (1.0/2.0)/2.0 :		1.0/2.0 = toggle every ½ second  .... divide by 2 again to get full pulse 
	m_rPulse = new CSulUpdateCallbackToggleNode( m_rLaserGroup, 0.0 );
	setUpdateCallback( m_rPulse );
}

void CSulGeomLaser::setPulseFrequency( double pulse )
{
	m_rPulse->setPulseFrequency( pulse );
}

osg::Group* CSulGeomLaser::getLaserNode()
{
	return m_rLaserGroup;
}

void CSulGeomLaser::setPosition( const osg::Vec3& v )
{
	m_vP = v;
	m_rPat->setPosition( v );
	calcRot();
}

void CSulGeomLaser::setTarget( const osg::Vec3& v )
{
	m_vT = v;
	osg::Vec3 tmp = v-m_vP;
	float fLen = tmp.length();
	m_rPat->setScale( osg::Vec3(1,fLen,1) );	
	calcRot();
}

osg::Vec3 CSulGeomLaser::getTarget()
{
	return m_vT;
}

void CSulGeomLaser::calcRot()
{
	osg::Quat q;
	osg::Vec3 v0 = m_vT-m_vP;
	osg::Vec3 v1( 0, 1, 0 );

	q.makeRotate( v1, v0 );
	m_rPat->setAttitude( q );
}

void CSulGeomLaser::enabled( bool bEnable )
{
	m_rPat->setNodeMask( bEnable ? 0xFFFFFFFF:0 );
}

void CSulGeomLaser::setRadius( float radius )
{
	m_rPat->removeChildren(0, 1);
	m_rPat->addChild( createBillboard(radius,radius) );
}

void CSulGeomLaser::setRadiusStart( float radius )
{
	m_rPat->removeChildren(0, 1);
	m_radiusStart = radius;
	m_rPat->addChild( createBillboard( m_radiusStart, m_radiusEnd ) );
}

void CSulGeomLaser::setRadiusEnd( float radius )
{
	m_rPat->removeChildren(0, 1);
	m_radiusEnd = radius;
	m_rPat->addChild( createBillboard( m_radiusStart, m_radiusEnd ) );
}

osg::Billboard* CSulGeomLaser::createBillboard( float radiusStart, float radiusEnd )
{
	osg::Billboard* pBillboard = new osg::Billboard;
	pBillboard->setMode( osg::Billboard::AXIAL_ROT );
	pBillboard->addDrawable( createQuad(radiusStart, radiusEnd) );
	pBillboard->setNormal( osg::Vec3(0,0,1) );
	pBillboard->setAxis( osg::Vec3(0,1,0) );
	return pBillboard;
}