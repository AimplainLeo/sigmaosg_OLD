// SulRenderCellInstances.cpp

#include "stdafx.h"
#include "SulRenderCellInstances.h"
#include "SulGeomCrossQuadInstancing.h"
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <iostream>

template <class T>
T nexthigher(T k) {
        if (k == 0)
                return 1;
        k--;
        for (int i=1; i<sizeof(T)*CHAR_BIT; i<<=1)
                k = k | k >> i;
        return k+1;
}

CSulRenderCellInstances::CSulRenderCellInstances( osg::Vec2 cellXY, osg::Image* pImagePositions, sigma::uint32 numInst, osg::BoundingBox& bb, float min, float max, bool bSuppressTexture, sigma::uint32 texUnit, bool bUseZDirectionNormal, bool bSuppressShaders ) : 
osg::Group(),
m_numInstances( numInst ),
m_rImagePositions( pImagePositions ),
m_bb( bb ),
m_bSuppressTexture( bSuppressTexture ),
m_texUnit( texUnit ),
m_min( min ),									// min height of tree
m_max( max ),									// max height of tree
m_bSuppressShaders( bSuppressShaders ),
m_bUseZDirectionNormal( bUseZDirectionNormal ),
m_cellXY( cellXY )
{


}

osg::ref_ptr<osg::Geode> CSulRenderCellInstances::createCrossQuadCell( sigma::uint32 x, sigma::uint32 y )
{
	float xmin = m_bb.xMin();
	float xmax = m_bb.xMax();
	float ymin = m_bb.yMin();
	float ymax = m_bb.yMax();
	float cellw = (xmax-xmin) / m_cellXY.x();
	float celld = (ymax-ymin) / m_cellXY.y();

	// calc bounding box for cell
	float xmin_cell = xmin+cellw*x;
	float xmax_cell = xmin_cell+cellw;
	float ymin_cell = ymin+celld*y;
	float ymax_cell = ymin_cell+celld;
	osg::BoundingBox bb( xmin_cell, ymin_cell, m_bb.zMin(), xmax_cell, ymax_cell, m_bb.zMax() );

	std::vector<osg::Vec3>	vecList;

	// find all the positions that are within the boundingbox of our cell
	float* p = reinterpret_cast<float*>(m_rImagePositions->data());
	for ( sigma::uint32 i=0; i<m_numInstances; i++ )
	{
		osg::Vec3 v;

		v.x() = p[0+i*3];
		v.y() = p[1+i*3];
		v.z() = p[2+i*3];

		if ( v.x()>xmin_cell && v.x()<xmax_cell && v.y()>ymin_cell && v.y()<ymax_cell )
		{
			vecList.push_back( v );
		}
	}

	// calc a good texture size for the positions
	sigma::uint32 sqr_root = sqrt( (float)vecList.size() )+1;
	sigma::uint32 texSizeSquared = nexthigher<sigma::uint32>( sqr_root );

	osg::ref_ptr<osg::Image> rImage = new osg::Image;
	rImage->allocateImage( texSizeSquared, texSizeSquared, 1,  GL_RGB, GL_FLOAT );	
	rImage->setInternalTextureFormat( GL_RGB32F_ARB );

	p = reinterpret_cast<float*>(rImage->data());

	osg::ref_ptr<osg::Texture2D> rTexturePositions = new osg::Texture2D( rImage );
	rTexturePositions->setInternalFormat( GL_RGB32F_ARB );
	rTexturePositions->setSourceFormat( GL_FLOAT );
	rTexturePositions->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	rTexturePositions->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);

	for ( sigma::uint32 i=0; i<vecList.size(); i++ )
	{
		p[i*3+0] = vecList[i].x();
		p[i*3+1] = vecList[i].y();
		p[i*3+2] = vecList[i].z();
	}

	// as usual

	CSulGeomCrossQuadInstancing* pCrossQuad = new CSulGeomCrossQuadInstancing( (int)vecList.size(), m_bUseZDirectionNormal );

	osg::StateSet* ss = pCrossQuad->getOrCreateStateSet();

	// texture positions
	ss->setTextureAttributeAndModes( 1, rTexturePositions, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	ss->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON );
	ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, 0.9f), osg::StateAttribute::ON );

	//ss->addUniform( new osg::Uniform( "use_tree_shader", 2 ) );

	osg::Uniform* tmp = 0;

	tmp = new osg::Uniform( osg::Uniform::SAMPLER_2D, "texPositions" );
	tmp->set( (int)1 );
	ss->addUniform( tmp );

	ss->addUniform( new osg::Uniform( "texSizeSquared", (int)texSizeSquared ) );
	ss->addUniform( new osg::Uniform( "numInstances", (int)vecList.size() ) );
	ss->addUniform( new osg::Uniform( "minSize", m_min ) );
	ss->addUniform( new osg::Uniform( "maxSize", m_max ) );

	// disable the computebound
	pCrossQuad->getDrawable( 0 )->setComputeBoundingBoxCallback(new osg::Drawable::ComputeBoundingBoxCallback);

	pCrossQuad->getDrawable( 0 )->setInitialBound( bb );

	return pCrossQuad;
}

void CSulRenderCellInstances::process()
{
	for ( sigma::uint32 y=0; y<m_cellXY.y(); y++ )
		for ( sigma::uint32 x=0; x<m_cellXY.x(); x++ )
		{
			osg::Node* p = createCrossQuadCell( x, y );
			addChild( p );
		}
}
