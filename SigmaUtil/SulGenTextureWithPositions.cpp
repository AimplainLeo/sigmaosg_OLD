// SulGenTextureWithPositions.cpp

#include "stdafx.h"
#include "SulGenTextureWithPositions.h"
#include "SulMath.h"
#include "SulIntLine.h"
#include "SulGeomLine.h"
#include "SulIntTriangle.h"
#include "sulDistancePointAndTriangle2d.h"
#include <osg/PositionAttitudeTransform>
#include <osg/io_utils>
#include <osgUtil/IntersectVisitor>

template <class T>
T nexthigher(T k) {
        if (k == 0)
                return 1;
        k--;
        for (int i=1; i<sizeof(T)*CHAR_BIT; i<<=1)
                k = k | k >> i;
        return k+1;
}

CSulGenTextureWithPositions::CSulGenTextureWithPositions(
	osg::PositionAttitudeTransform*	pSceneTerrain,
	const sigma::VEC_LINESEGMENT&	line,
	const sigma::VEC_TRI&			tri,
	float							radius,
	float							distance_between_trees_line,
	float							areaPadding,
	VEC_GENPOSITIONMASK				vecMask
) :
m_rSceneTerrain( pSceneTerrain ),
m_vecLine( line ),
m_vecTri( tri ),
m_radius( radius ),
m_distance_between_trees_line( distance_between_trees_line ),
m_areaPadding( areaPadding ),
m_vecMask( vecMask )
{
	process();
}

void CSulGenTextureWithPositions::process()
{
	processLines();
	processTriangles();
	processMasks();
	processTexture();
}

void CSulGenTextureWithPositions::processLine( osg::LineSegment* pLine )
{
	// calc number of trees on line segment
	osg::Vec3 vSeg = pLine->end()-pLine->start();
	float len = vSeg.length();
	sigma::uint32 numTrees = (len/m_distance_between_trees_line)+1;

	// calc new spacing
	float spc = len/(float)numTrees;

	vSeg.normalize();

	// HACK: the +1 fills in the last spot on the line segment
	// apparently the math to calculate the number of trees isnt' right :(
	for ( sigma::uint32 i=0; i<numTrees+1; i++ )
	{
		float z = 0.0f;
		osg::Vec3 plant_pos = pLine->start() + vSeg*(spc*i);
	
		// we need to calculate a true z position
		// plant_pos is local to scene
		osg::LineSegment* ls = new osg::LineSegment;
		osg::Vec3 s = plant_pos;
		osg::Vec3 e = plant_pos;
		s.z() = 1000.0f;
		e.z() = -1000.0f;
		ls->set( s, e );

		osgUtil::IntersectVisitor iv;
		iv.addLineSegment( ls );
		m_rSceneTerrain->accept( iv );
		if ( iv.hits() )
		{
			osgUtil::IntersectVisitor::HitList hitList = iv.getHitList( ls );
			osg::Vec3 hit = hitList[0].getWorldIntersectPoint();

			z = hit.z(); 

			m_vecPos.push_back( osg::Vec3(plant_pos.x(), plant_pos.y(), z) );
		}
		else
		{
			osg::notify(osg::NOTICE) << "missed (line)" << std::endl;
		}
	}
}

void CSulGenTextureWithPositions::processLines()
{
	sigma::VEC_LINESEGMENT::const_iterator i;
	i = m_vecLine.begin();
	while ( i!=m_vecLine.end() )
	{
		processLine( *i );
		++i;
	}
}

void CSulGenTextureWithPositions::processTriangles()
{
	// calc area of each triangle
	std::vector<float> vecA;
	vecA.reserve( m_vecTri.size() );
	float sum = 0.0f;
	sigma::VEC_TRI::const_iterator i_const;
	i_const = m_vecTri.begin();
	while ( i_const!=m_vecTri.end() )
	{
		float a = i_const->getArea();;
		vecA.push_back( a );
		sum += a;
		++i_const;
	}
		
	// normalize area for each triangle
	std::transform( 
		vecA.begin(), vecA.end(), 
		vecA.begin(),
		std::bind2nd(std::divides<float>(), sum) );

	// calc number of trees to plant
	sigma::uint32 numTrees = (sum*0.907f)/(osg::PI*m_radius*m_radius);

	// find random triangle and point inside that triange as the position
	sigma::VEC_VEC3 vecPositions;
	for ( sigma::uint32 i=0; i<numTrees; i++ )
	{
		float t = sigma::rand0to1();

		// which triangle to use (j is the triangle index)
		unsigned int j = 0;
		float s = 0.0f;
		while ( j < vecA.size() )
		{
			s += vecA[j];
			if ( s > t )
				break;

			++j;
		}

		vecPositions.push_back( m_vecTri[j].getRandomPoint() );
	}

	// we need to clamp the z values for the positions, because the shape file may not be aligned correctly in 
	// the z direction

	sigma::VEC_VEC3::iterator iPos;
	iPos = vecPositions.begin();
	while ( iPos!=vecPositions.end() )
	{
		// create a line that extens above and below the position
		osg::LineSegment* ls = new osg::LineSegment;
		osg::Vec3 s = *iPos;
		osg::Vec3 e = *iPos;
		s.z() = 1000.0f;
		e.z() = -1000.0f;
		ls->set( s, e );

		// intersect with scene
		osgUtil::IntersectVisitor iv;
		iv.addLineSegment( ls );
		m_rSceneTerrain->accept( iv );
		if ( iv.hits() )
		{
			osgUtil::IntersectVisitor::HitList hitList = iv.getHitList( ls );
			osg::Vec3 hit = hitList[0].getWorldIntersectPoint();

			m_vecPos.push_back( 				
				osg::Vec3(
					iPos->x(), 
					iPos->y(), 
					 hit.z() )		
				);
		}
		else
		{
			osg::notify(osg::NOTICE)  << "missed (poly)" << std::endl;
		}

		++iPos;
	}
}

void CSulGenTextureWithPositions::processTexture()
{
	sigma::VEC_VEC3::iterator i;

	// calc a good texture size for the positions
	sigma::uint32 sqr_root = sqrt( (float)m_vecPos.size() )+1;
	m_texSizeSquared = nexthigher<sigma::uint32>( sqr_root );


	m_rImage = new osg::Image;
	m_rImage->allocateImage( m_texSizeSquared, m_texSizeSquared, 1,  GL_RGB, GL_FLOAT );	
	m_rImage->setInternalTextureFormat( GL_RGB32F_ARB );

	float* p = reinterpret_cast<float*>(m_rImage->data());

	osg::NodePathList parentNodePathList = m_rSceneTerrain->getParentalNodePaths();
	osg::Matrix mWL = osg::computeWorldToLocal(parentNodePathList[0]);
	osg::notify(osg::NOTICE) << mWL.getTrans() << std::endl;

	m_posCount = 0;
	i = m_vecPos.begin();
	while ( i!=m_vecPos.end() )
	{
		// convert positions to terrain local space
		osg::Vec3 v = (*i)*mWL;

		p[0+m_posCount*3] = v.x();
		p[1+m_posCount*3] = v.y();;
		p[2+m_posCount*3] = v.z();
		++m_posCount;

		++i;
	}
}

osg::Image*	CSulGenTextureWithPositions::getImage()
{
	return m_rImage;
}

sigma::uint32 CSulGenTextureWithPositions::getCount()
{
	return m_posCount;
}

sigma::uint32 CSulGenTextureWithPositions::getTexSizeSquared()
{
	return m_texSizeSquared;
}

void CSulGenTextureWithPositions::processMasks()
{
	osg::notify(osg::NOTICE) << "mask count: " << m_vecMask.size() << std::endl;

	VEC_GENPOSITIONMASK::iterator iMask = m_vecMask.begin();
	while ( iMask!=m_vecMask.end() )
	{
		processMask( iMask->get() );		
		++iMask;
	}
}

void CSulGenTextureWithPositions::processMaskLines( CSulGenPositionMask* pMask )
{
	if ( pMask->m_vecLineMask.size() )
	{
		sigma::VEC_VEC3 vecPos;
		sigma::VEC_LINESEGMENT::const_iterator i;

		i = pMask->m_vecLineMask.begin();
		while ( i!=pMask->m_vecLineMask.end() )
		{
			if ( m_vecPos.size() )
			{
				sigma::VEC_VEC3::iterator iPos = m_vecPos.begin();
				while ( iPos!=m_vecPos.end() )
				{
					osg::Vec3 s = *iPos;
					osg::Vec3 e = *iPos;
					s.z() -= 1000.0f;
					e.z() += 1000.0f;
					osg::ref_ptr<osg::LineSegment> line = new osg::LineSegment( s, e );

					float dist = SulIntLine( *(*i), *line );
					if (  dist < pMask->m_lineDist )
					{
						osg::notify(osg::NOTICE) << "masked: " << dist << std::endl;

						/* DEBUGGING: add a line to show where the tree was taken out */
/*
						CSulGeomLine* pLine = new CSulGeomLine( *line );
						osg::Geode* geode = new osg::Geode;
						geode->addDrawable( pLine->getDrawable() );
						m_rSceneTerrain->getParent(0)->addChild( geode );
*/
						/**/

						// discard this position
						++iPos;
						continue;
					}

					vecPos.push_back( *iPos );

					++iPos;
				}

				m_vecPos = vecPos;
				vecPos.clear();
			}	

			++i;
		}
	}
}

void CSulGenTextureWithPositions::processMaskTri( CSulGenPositionMask* pMask )
{
	if ( pMask->m_vecTriMask.size() ) 
	{
		sigma::VEC_VEC3 vecPos;
		sigma::VEC_TRI::const_iterator i;

		i = pMask->m_vecTriMask.begin();
		while ( i!=pMask->m_vecTriMask.end() )
		{
			if ( m_vecPos.size() )
			{
				sigma::VEC_VEC3::iterator iPos = m_vecPos.begin();
				while ( iPos!=m_vecPos.end() )
				{
					osg::Vec3 s = *iPos;
					osg::Vec3 e = *iPos;
					s.z() -= 1000.0f;
					e.z() += 1000.0f;
					osg::ref_ptr<osg::LineSegment> line = new osg::LineSegment( s, e );

					// if line intersects triangle then we remove the position
					if ( sulIntTriangle( *line, (*i) ) )
					{
						osg::notify(osg::NOTICE) << "masked with tri " << std::endl;						
						++iPos;
						continue;
					}

					// if the line/point is within 2 meters (debugging with 2 meters) of the triangle we remove the position
					double d = sulDistancePointAndTriangle2d( s, (*i) );
					if ( d<m_areaPadding )
					{
						osg::notify(osg::NOTICE) << "masked with distance tri and point d=" << d << std::endl;						
						++iPos;
						continue;
					}

					vecPos.push_back( *iPos );

					++iPos;
				}

				m_vecPos = vecPos;
				vecPos.clear();
			}

			++i;
		}
	}
}

void CSulGenTextureWithPositions::processMask( CSulGenPositionMask* pMask )
{
	processMaskLines( pMask );
	processMaskTri( pMask );

	osg::notify(osg::NOTICE) << "trees count: " << m_vecPos.size() << std::endl;
}

const sigma::VEC_VEC3& CSulGenTextureWithPositions::getPositions()
{
	return m_vecPos;
}

