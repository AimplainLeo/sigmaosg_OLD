// SceneBase.cpp

#include "stdafx.h"
#include "SceneBase.h"
#include <SigmaUtil/SulGeomBoundingBox.h>
#include <SigmaUtil/SulClipLines.h>
#include <SigmaUtil/SulClipTriangle.h>
#include <SigmaUtil/SulGeomLineList.h>
#include <SigmaUtil/SulGeomTriangleList.h>
#include <osg/geode>
#include <osgDB/ReadFile>

CSceneBase::CSceneBase( const CSulString& sFile, bool bRenderMe, const osg::Vec4& clipColor ) :
m_clipColor( clipColor ),
m_bRenderMe( bRenderMe )
{
	m_rPat = new osg::PositionAttitudeTransform;
	m_rScene = osgDB::readNodeFile( sFile );
	if ( m_rScene.valid() )
	{
		m_rPat->addChild( m_rScene );
		m_loc = dynamic_cast<osgSim::GeographicLocation*>(m_rScene->getUserData());
		addChild( m_rPat );

		if ( m_loc.valid() )
		{
			m_rCoordLatLon = new CSulCoordLatLon( m_loc->latitude(), m_loc->longitude() );
		}
		
		m_rComputeBounds = new osg::ComputeBoundsVisitor;
		m_rScene->accept( *m_rComputeBounds );
	}
	else
	{
		osg::notify(osg::NOTICE) << "CSceneBase can't find file: " << sFile << std::endl;
	}
}

double CSceneBase::getLat()
{
	return m_loc->latitude();
}

double CSceneBase::getLon()
{
	return m_loc->longitude();
}

CSulCoordLatLon* CSceneBase::getCoordLatLonObject()
{
	return m_rCoordLatLon;
}

void CSceneBase::adjustPosition( CSulCoordUTM* pWorldUTM )
{
	if ( m_rCoordLatLon.valid() )
	{
		osg::Vec3 v = pWorldUTM->getXYZ( *m_rCoordLatLon );
		m_rPat->setPosition( v );
	}
}

void CSceneBase::addOffset( float x, float y, float z )
{
	osg::Vec3 v = m_rPat->getPosition();
	v += osg::Vec3( x, y, z );
	m_rPat->setPosition( v );
}

void CSceneBase::drawBoundingBox()
{
	if ( m_rComputeBounds.valid() )
	{
		osg::BoundingBox bb = m_rComputeBounds->getBoundingBox();
		CSulGeomBoundingBox* gbb = new CSulGeomBoundingBox( bb );
		m_rPat->addChild( gbb );
	}
}

sigma::VEC_PLANE CSceneBase::getBoundingBoxPlanes()
{
	osg::Polytope polyTope;
	m_rComputeBounds->getPolytope( polyTope, 0.0f );

	// transform the planes to world space
	sigma::VEC_PLANE tmp = polyTope.getPlaneList();
	osg::Matrix m;
	m_rPat->computeLocalToWorldMatrix( m, 0 );
	sigma::VEC_PLANE::iterator i;
	i = tmp.begin();
	while ( i!=tmp.end() )
	{
		i->transform( m );
		++i;
	}

	return tmp;
}

void CSceneBase::clip( const sigma::VEC_PLANE& vecPlane )
{
	// create a clipped version of the shape file that in contained inside a given boundingbox
	CSulExtractGeometry sceneGeometry;
	m_rScene->accept( sceneGeometry );

	clipLines( sceneGeometry, vecPlane );
	clipTriangles( sceneGeometry, vecPlane );
}

void CSceneBase::clipLines( const CSulExtractGeometry& sceneGeometry, const sigma::VEC_PLANE& vecPlane )
{
	m_vecLines = sceneGeometry.getLineSegmentList();

	osg::Matrix mWL;
	m_rPat->computeWorldToLocalMatrix( mWL, 0 );

	osg::notify(osg::NOTICE) << "clippling lines against plane" << std::endl;
	sigma::uint32 count = 0;
	sigma::VEC_PLANE::const_iterator i;
	i = vecPlane.begin();
	while ( i!=vecPlane.end() )
	{
		osg::Plane plane = (*i);
		plane.transform( mWL );

		// hack: we need to extend the top and bottom distance of the plane so it can contain the shape files geometry
		if ( count==0 || count==1 ) 
			plane[3] += 1000.0f;
		
		CSulClipLines clipLines( m_vecLines, plane );
		m_vecLines = clipLines.getLinesList();
		osg::notify(osg::NOTICE)  << "clip lines[" << count++ << "]: " << m_vecLines.size() << std::endl;
		++i;
	}

	if ( m_vecLines.size() )
	{
		CSulGeomLineList* pGeomLines = new CSulGeomLineList( m_vecLines );
		pGeomLines->setColor( m_clipColor );
		m_rPat->addChild( pGeomLines );
	}
}

void CSceneBase::clipTriangles( const CSulExtractGeometry& sceneGeometry, const sigma::VEC_PLANE& vecPlane )
{
	m_vecTriangles = sceneGeometry.getTriList();

	osg::Matrix mWL;
	m_rPat->computeWorldToLocalMatrix( mWL, 0 );

	osg::notify(osg::NOTICE) << "clippling triangles against plane" << std::endl;
	sigma::uint32 count = 0;
	sigma::VEC_PLANE::const_iterator i;
	i = vecPlane.begin();
	while ( i!=vecPlane.end() )
	{
		osg::Plane plane = (*i);
		plane.transform( mWL );

		// hack: we need to extend the top and bottom distance of the plane so it can contain the shape files geometry
		if ( count==0 || count==1 ) 
			plane[3] += 1000.0f;
		
		CSulClipTriangle clipTris( m_vecTriangles, plane );
		m_vecTriangles = clipTris.getTriangleList();
		osg::notify(osg::NOTICE)  << "clip triangles[" << count++ << "]: " << m_vecTriangles.size() << std::endl;
		++i;
	}

	if ( m_vecTriangles.size() )
	{
		osg::PositionAttitudeTransform* pPat = new osg::PositionAttitudeTransform;
		pPat->setPosition( osg::Vec3(0,0,20) );

		CSulGeomTriangleList* pGeomTris = new CSulGeomTriangleList( m_vecTriangles );
		pGeomTris->setColor( m_clipColor );
		
		osg::Geode* pGeodeTris = new osg::Geode;
		pGeomTris->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
		pGeomTris->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
		pPat->addChild( pGeomTris );
		m_rPat->addChild( pPat );
	}
}

const sigma::VEC_LINESEGMENT& CSceneBase::getClippedLineList()
{
	return m_vecLines;
}

const sigma::VEC_TRI& CSceneBase::getClippedTriangleList()
{
	return m_vecTriangles;
}

osg::PositionAttitudeTransform* CSceneBase::getPat()
{
	return m_rPat;
}

osg::ComputeBoundsVisitor* CSceneBase::getBoundsVisitor()
{
	return m_rComputeBounds;
}

osg::BoundingBox CSceneBase::getBoundingBoxWorld()
{
	osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
	//m_rPat->accept( *tmp );
	//accept( *tmp );
	m_rScene->accept( *tmp );
	return 	tmp->getBoundingBox();
}

bool CSceneBase::isRenderMe()
{
	return m_bRenderMe;
}