// SulGenPositionTextures.cpp

#include "stdafx.h"
#include "SulGenPositionTextures.h"
#include "SulMath.h"
#include "SulIntLine.h"
#include "SulClipLines.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include <osg/io_utils>
#include <osgUtil/IntersectVisitor>
#include <osg/PositionAttitudeTransform>

template <class T>
T nexthigher(T k) {
        if (k == 0)
                return 1;
        k--;
        for (int i=1; i<sizeof(T)*CHAR_BIT; i<<=1)
                k = k | k >> i;
        return k+1;
}

CSulGenPositionTextures::CSulGenPositionTextures()
{
}

CSulGenPositionTextures::CSulGenPositionTextures( 
	osg::Node*				pScene,
	osg::Matrix				lineTransform,
	osg::Node*				pShape,
	const osg::BoundingBox&	bb,
	const std::vector<osg::Plane> vecPlane,
	const VEC_DATASHAPEMASK& vecDataShapeMask,
	float					lineSpacing,
	float					lineJitterRadius,
	bool					lineSnap
) :
m_rScene( pScene ),
m_lineTransform(lineTransform),
m_rShape( pShape ),
m_bb( bb ),
m_vecPlane( vecPlane ),
m_vecDataShapeMask( vecDataShapeMask ),
m_lineSpacing( lineSpacing ),
m_lineJitterRadius( lineJitterRadius ),
m_lineSnap( lineSnap ),
m_radius( 2.0f ),
m_texSizeSquared( 0 )
{
	// HACK: adjust min and max z values for bounding box so line segments can intersect
	m_bb._min.z() = -100.0f;
	m_bb._max.z() = 100.0f;

	m_vecPlane[0][3] += 1000.0f;
	m_vecPlane[1][3] += 1000.0f;

	std::cout << "\nmin= " << m_bb._min << "\nmax= " << m_bb._max << std::endl;

	// pre-process masks
	preprocessMasks();
}

void CSulGenPositionTextures::addLine( const osg::LineSegment& line )
{
	float r1;
	float r2;

	// check if line intersects boundingbox
	if ( line.intersect( m_bb, r1, r2 ) )
	{
		// calc intersection end points
		osg::Vec3 v = line.end()-line.start();
		osg::Vec3 p1 = line.start() + v*r1;
		osg::Vec3 p2 = line.end() - v*r2;
		osg::Vec3 vSeg = p2-p1;
	
		// calc number of trees on line segment
		float len = vSeg.length();
		sigma::uint32 numTrees = (len/m_lineSpacing)+1;

		// calc new spacing
		float spc = len/(float)numTrees;

		vSeg.normalize();

		// HACK: the +1 fills in the last spot on the line segment
		// apparently the math to calculate the number of trees isnt' right :(
		for ( sigma::uint32 i=0; i<numTrees+1; i++ )
		{
			float z = 0.0f;
			osg::Vec3 plant_pos = p1 + vSeg*(spc*i);
		
			// we need to calculate a true z position
			// plant_pos is local to scene
			osg::LineSegment* ls = new osg::LineSegment;
			osg::Vec3 s = plant_pos;
			osg::Vec3 e = plant_pos;
			s.z() = 1000.0f;
			e.z() = -1000.0f;
			ls->set( s, e );

			osg::Matrix mLocalToWorld;
			osg::PositionAttitudeTransform* pPat = dynamic_cast<osg::PositionAttitudeTransform*>(m_rScene.get());
			pPat->computeLocalToWorldMatrix( mLocalToWorld, 0 );
			osg::ref_ptr<osg::LineSegment> rLS_New = new osg::LineSegment;
			rLS_New->mult( *ls , mLocalToWorld ); 

			osgUtil::IntersectVisitor iv;
			iv.addLineSegment( rLS_New );
			m_rScene->accept( iv );
			if ( iv.hits() )
			{
				osgUtil::IntersectVisitor::HitList hitList = iv.getHitList( rLS_New );
				osg::Vec3 hit = hitList[0].getWorldIntersectPoint();

				z = hit.z(); 

				addTree( osg::Vec3(plant_pos.x(), plant_pos.y(), z) );
			}
			else
			{
				std::cout << "missed" << std::endl;
			}


		}

		m_lineCount++;
	}
}

void CSulGenPositionTextures::process()
{
	m_lineCount = 0;
	m_posCount = 0;

	// create a list of lines from shape file 
	CSulNodeVisitorExtractLines myLines;
	m_rShape->accept( myLines );
	const sigma::VEC_LINESEGMENT& lines = myLines.getLineSegmentList();
	sigma::VEC_LINESEGMENT::const_iterator i;

	// now process each line into data for our x,y positions for grass/trees
	i = lines.begin();
	while ( i!=lines.end() )
	{
		osg::ref_ptr<osg::LineSegment> newLine = new osg::LineSegment;
		newLine->mult( *i->get(), m_lineTransform );
		addLine( *newLine );
		++i;
	}

	////////////////////////
	// polygon
	////////////////////////

	// now process each triangle
	sigma::VEC_TRI tri_org = myLines.getTriList();

	if ( tri_org.size() )
	{
		sigma::VEC_TRI::iterator iTri;

		// transform tri to local space
		iTri = tri_org.begin();
		while ( iTri!=tri_org.end() )
		{
			iTri->m_p0 = iTri->m_p0*m_lineTransform;
			iTri->m_p1 = iTri->m_p1*m_lineTransform;
			iTri->m_p2 = iTri->m_p2*m_lineTransform;
			++iTri;
		}

		// make new set of triangles (remove triangles not in bounding box,. need to clip at some point)

		CSulClipTriangle clipTri0( tri_org, m_vecPlane[0] );
		CSulClipTriangle clipTri1( clipTri0.getTriangleList(), m_vecPlane[1] );
		CSulClipTriangle clipTri2( clipTri1.getTriangleList(), m_vecPlane[2] );
		CSulClipTriangle clipTri3( clipTri2.getTriangleList(), m_vecPlane[3] );
		CSulClipTriangle clipTri4( clipTri3.getTriangleList(), m_vecPlane[4] );
		m_clipTri = new CSulClipTriangle( clipTri4.getTriangleList(), m_vecPlane[5] );

		std::cout << "clipTri0 = " << clipTri0.getCount() << std::endl;
		std::cout << "clipTri1 = " << clipTri1.getCount() << std::endl;
		std::cout << "clipTri2 = " << clipTri2.getCount() << std::endl;
		std::cout << "clipTri3 = " << clipTri3.getCount() << std::endl;
		std::cout << "clipTri4 = " << clipTri4.getCount() << std::endl;
		std::cout << "clipTri5 = " << m_clipTri->getCount() << std::endl;

		const sigma::VEC_TRI& tri = m_clipTri->getTriangleList();
		sigma::VEC_TRI::const_iterator i_const;

		// calc area of polygon
		std::vector<float> vecA;
		vecA.reserve( tri.size() );
		float sum = 0.0f;
		i_const = tri.begin();
		while ( i_const!=tri.end() )
		{
			float a = i_const->getArea();;
			vecA.push_back( a );
			sum += a;
			++i_const;
		}
		
		std::transform( vecA.begin(), vecA.end(), vecA.begin(),
			std::bind2nd(std::divides<float>(), sum) );

		float test = std::accumulate(vecA.begin(), vecA.end(), 0.f);
		osg::notify(osg::ALWAYS) << "sum of normalized array = " << test << std::endl;
		
		osg::notify(osg::ALWAYS) << "total area = " << sum << std::endl;

		// calc number of trees to plant
		sigma::uint32 n = (sum*0.907f)/(osg::PI*m_radius*m_radius);

		float density = n/sum;
		osg::notify(osg::ALWAYS) << "density = " << density << std::endl;


		plantTrees( n, vecA, tri );	
	}

	////////////////////////
	// other stuff
	////////////////////////

	processTexture();
}

void CSulGenPositionTextures::plantTrees( sigma::uint32 numTrees, const std::vector<float>& vecA, const sigma::VEC_TRI& tri )
{
	for ( sigma::uint32 i=0; i<numTrees; i++ )
	{
		float t = sigma::rand0to1();

		// which triangle to place tree into (j is the triangle index)
		unsigned int j = 0;
		float s = 0.0f;
		while ( j < vecA.size() )
		{
			s += vecA[j];
			if ( s > t )
				break;

			++j;
		}

		addTri( tri[j] );
	}

	// put positions into texture positions
	std::vector<osg::Vec3>::iterator iPos;
	sigma::uint32 ccc = 0;
	iPos = m_vecPositions.begin();
	while ( iPos!=m_vecPositions.end() )
	{
		osg::LineSegment* ls = new osg::LineSegment;
		osg::Vec3 s = *iPos;
		osg::Vec3 e = *iPos;
		s.z() = 1000.0f;
		e.z() = -1000.0f;
		ls->set( s, e );

		osg::Matrix mLocalToWorld;
		osg::PositionAttitudeTransform* pPat = dynamic_cast<osg::PositionAttitudeTransform*>(m_rScene.get());
		pPat->computeLocalToWorldMatrix( mLocalToWorld, 0 );
		osg::ref_ptr<osg::LineSegment> rLS_New = new osg::LineSegment;
		rLS_New->mult( *ls , mLocalToWorld ); 

		osgUtil::IntersectVisitor iv;
		iv.addLineSegment( rLS_New );
		m_rScene->accept( iv );
		if ( iv.hits() )
		{
			osgUtil::IntersectVisitor::HitList hitList = iv.getHitList( rLS_New );
			osg::Vec3 hit = hitList[0].getWorldIntersectPoint();

			addTree(
				osg::Vec3(
					iPos->x(), 
					iPos->y(), 
					 hit.z() )
			);
		}
		else
		{
			std::cout << "missed" << std::endl;
		}

		std::cout << "plantTrees (" << m_vecPositions.size() << "/" << ccc++ << ")" << std::endl;

		++iPos;
	}

}

void CSulGenPositionTextures::addTri( CSulDataTri tri )
{
	float a = sigma::rand0to1();
	float b = sigma::rand0to1();

	if ( (a+b) > 1.0f )
	{
		a = 1.0f-a;
		b = 1.0f-b;
	}

	osg::Vec3 AB = tri.m_p0-tri.m_p2;
	osg::Vec3 AC = tri.m_p1-tri.m_p2;

	osg::Vec3 P = tri.m_p2+AB*a+AC*b;
	m_vecPositions.push_back( P );
}

sigma::uint32 CSulGenPositionTextures::getTexSizeSquared()
{
	return m_texSizeSquared;
}

void CSulGenPositionTextures::processTexture()
{
	sigma::VEC_VEC3::iterator i;

	// calc a good texture size for the positions
	sigma::uint32 sqr_root = sqrt( (float)m_vecPos.size() )+1;
	m_texSizeSquared = nexthigher<sigma::uint32>(sqr_root);


	m_rImage = new osg::Image;
	m_rImage->allocateImage( m_texSizeSquared, m_texSizeSquared, 1,  GL_RGB, GL_FLOAT );	
	m_rImage->setInternalTextureFormat( GL_RGB32F_ARB );

	float* p = reinterpret_cast<float*>(m_rImage->data());

	i = m_vecPos.begin();
	while ( i!=m_vecPos.end() )
	{
		p[0+m_posCount*3] = i->x();
		p[1+m_posCount*3] = i->y();;
		p[2+m_posCount*3] = i->z();
		++m_posCount;

		++i;
	}
}

sigma::uint32 CSulGenPositionTextures::getCount()
{
	return m_lineCount;
}

sigma::uint32 CSulGenPositionTextures::getCountPos()
{
	return m_posCount;
}

osg::Image*	CSulGenPositionTextures::getImage()
{
	return m_rImage;
}

const CSulClipTriangle& CSulGenPositionTextures::getClipTriangles()
{
	return *m_clipTri;
}

const std::vector<osg::Vec3>& CSulGenPositionTextures::getTreePositions() const
{
	return m_vecPos;
}

void CSulGenPositionTextures::preprocessMasks()
{
	VEC_DATASHAPEMASK::iterator i;

	i = m_vecDataShapeMask.begin();
	while ( i!=m_vecDataShapeMask.end() )
	{
		osg::Node* node = i->m_node;		

		////////////////////////////////////////////
		// process lines
		////////////////////////////////////////////

		CSulNodeVisitorExtractLines myLines;
		node->accept( myLines );
		const sigma::VEC_LINESEGMENT& lines_org = myLines.getLineSegmentList();
		sigma::VEC_LINESEGMENT::const_iterator ii;

		ii = lines_org.begin();
		while ( ii!=lines_org.end() )
		{
			(*ii)->mult( *(*ii), m_lineTransform );
			++ii;
		}

		// we need to minize the number of lines, so we only have those in the boundingbox area
		CSulClipLines clipLines0( lines_org, m_vecPlane[0] );
		CSulClipLines clipLines1( clipLines0.getLinesList(), m_vecPlane[1] );
		CSulClipLines clipLines2( clipLines1.getLinesList(), m_vecPlane[2] );
		CSulClipLines clipLines3( clipLines2.getLinesList(), m_vecPlane[3] );
		CSulClipLines clipLines4( clipLines3.getLinesList(), m_vecPlane[4] );
		i->m_clipLines = new CSulClipLines( clipLines4.getLinesList(), m_vecPlane[5] );

		const sigma::VEC_LINESEGMENT& lines = i->m_clipLines->getLinesList();

		std::cout << "mask lines org: " << lines_org.size() << std::endl;

		std::cout << 
			"clipLines0 " <<
			"mask lines clipped: "	<< clipLines0.getLineCount() << 
			std::endl;

		std::cout << 
			"clipLines1 " <<
			"mask lines clipped: "	<< clipLines1.getLineCount() << 
			std::endl;

		std::cout << 
			"clipLines2 " <<
			"mask lines clipped: "	<< clipLines2.getLineCount() << 
			std::endl;

		std::cout << 
			"clipLines3 " <<
			"mask lines clipped: "	<< clipLines3.getLineCount() << 
			std::endl;

		std::cout << 
			"clipLines4 " <<
			"mask lines clipped: "	<< clipLines4.getLineCount() << 
			std::endl;

		std::cout << 
			"clipLines5 " <<
			"mask lines clipped: "	<< i->m_clipLines->getLineCount() << 
			std::endl;

		++i;
	}
}
  
void CSulGenPositionTextures::addTree( const osg::Vec3& pos_local )
{
	// we need to check if this position needs to be discard because of roads, houses and stuff like that

	osg::Vec3 pos; // world
	osg::Matrix mLocalToWorld;
	osg::PositionAttitudeTransform* pPat = dynamic_cast<osg::PositionAttitudeTransform*>(m_rScene.get());
	pPat->computeLocalToWorldMatrix( mLocalToWorld, 0 );
	pos = mLocalToWorld*pos_local;

	// create a line that goes through pos
	osg::Vec3 p0 = pos;
	osg::Vec3 p1 = pos;
	p0.z()-=1000.0f;
	p1.z()+=1000.0f;

	VEC_DATASHAPEMASK::iterator i;

	i = m_vecDataShapeMask.begin();
	while ( i!=m_vecDataShapeMask.end() )
	{
		osg::Node* node = i->m_node;		

		sigma::VEC_LINESEGMENT::const_iterator ii;

		const sigma::VEC_LINESEGMENT& lines = i->m_clipLines->getLinesList();

		// now process each line
		ii = lines.begin();
		while ( ii!=lines.end() )
		{
			osg::ref_ptr<osg::LineSegment> line0 = new osg::LineSegment( (*ii)->start(), (*ii)->end() );
			osg::ref_ptr<osg::LineSegment> line1 = new osg::LineSegment( p0, p1 );

			if ( SulIntLine( *line0, *line1 ) < (i->m_lineDist) )
			{
				m_vecRejectedPositions.push_back( pos_local );
				// reject tree position, because it's within out mask
				return;
			}

			++ii;
		}

		++i;
	}

	// add tree
	m_vecPos.push_back( pos_local );
}

const sigma::VEC_VEC3& CSulGenPositionTextures::getRejectedPositionsList()
{
	return m_vecRejectedPositions;
}
