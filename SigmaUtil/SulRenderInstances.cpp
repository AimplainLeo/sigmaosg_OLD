// SulRenderInstances.cpp

#include "stdafx.h"
#include "SulRenderInstances.h"
#include "SulGeomBillboardInstancing.h"
#include "SulShaderInstancingBillboards.h"
#include <osg/copyop>
#include <osg/shapedrawable>
#include <osg/io_utils>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <iostream>
#include <assert.h>

bool SulRenderInstances_readLocalData(osg::Object& obj, osgDB::Input& fr)
{
	CSulRenderInstances& ri = static_cast<CSulRenderInstances&>(obj);
/*
	sigma::uint32 numInstances;
	osg::Vec3 minBound;
	osg::Vec3 maxBound;

	// TODO: handle error
	fr.readSequence( "num_instances", numInstances );	
	fr.readSequence( "min_bound", minBound );	
	fr.readSequence( "max_bound", maxBound );	
	
*/

	//ri.create();
	return true;
}

bool SulRenderInstances_writeLocalData(const osg::Object& obj, osgDB::Output& fw)
{
    const CSulRenderInstances& ri = static_cast<const CSulRenderInstances&>(obj);
/*
	fw.indent() << "num_instances " << ri.getNumInstances() << std::endl;
	fw.indent() << "min_bound " << ri.getBB()._min << std::endl;
	fw.indent() << "max_bound " << ri.getBB()._max << std::endl;
	//fw.indent() << "tex_positions " << ri.getTexturePositions() << std::endl;
*/
	return true;
}
/*
REGISTER_DOTOSGWRAPPER(CSulRenderInstances)
(
    new CSulRenderInstances,
    "CSulRenderInstances",
    "Object Node Geode CSulRenderInstances",
    &SulRenderInstances_readLocalData,
    &SulRenderInstances_writeLocalData
);
*/

CSulRenderInstances::CSulRenderInstances() :
osg::Group()
{
}

CSulRenderInstances::CSulRenderInstances( const CSulRenderInstances& ri, const osg::CopyOp& copyop ) :
osg::Group( ri, copyop )
{
	m_bSuppressTexture = ri.m_bSuppressTexture;
	m_numInstances = ri.m_numInstances;
	m_bb = ri.m_bb;
	m_rTexturePositions = static_cast<osg::Texture2D*>(copyop(ri.m_rTexturePositions.get()));
}

CSulRenderInstances::CSulRenderInstances( 
	osg::Image* pImagePositions, 
	sigma::uint32 numInst, 
	osg::BoundingBox& bb, 
	float min, 
	float max,
	bool bSuppressTexture,
	sigma::uint32 texUnit,
	sigma::uint32 texSizeSquared,
	sigma::uint32 useLights,
	bool bUseZDirectionNormal
) :
osg::Group(),
m_bb( bb ),
m_bSuppressTexture( bSuppressTexture ),
m_texUnit( texUnit ),
m_texSizeSquared( texSizeSquared ),
m_useLights( useLights ),
m_min( min ),
m_max( max ),
m_bUseZDirectionNormal( bUseZDirectionNormal )
{
	m_rTexturePositions = new osg::Texture2D( pImagePositions );
	m_rTexturePositions->setInternalFormat( GL_RGB32F_ARB );
	m_rTexturePositions->setSourceFormat( GL_FLOAT );
	m_rTexturePositions->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	m_rTexturePositions->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
	m_numInstances = numInst;
}

void CSulRenderInstances::create()
{
	CSulGeomBillboardInstancing* pBillboard = new CSulGeomBillboardInstancing( m_numInstances, m_bUseZDirectionNormal );

	addChild( pBillboard );

	pBillboard->getDrawable( 0 )->setInitialBound( m_bb );

	osg::StateSet* ss = getOrCreateStateSet();

	// texture positions
	ss->setTextureAttributeAndModes( 1, m_rTexturePositions, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

	if ( !m_bSuppressTexture )
	{
		// texture tree
		//std::string sFileTree = osgDB::findDataFile( "images/tree_aligned_256.png" );
		std::string sFileTree = osgDB::findDataFile( "images/trees.png" );
		if ( sFileTree.empty() )
		{
			std::cout << "WARNING: CSulRenderInstances::create can not find image " << sFileTree << std::endl;
		}
		else
		{
			osg::Image* pImageTree = osgDB::readImageFile( sFileTree );
			osg::Texture2D* pTex = new osg::Texture2D( pImageTree );
			pTex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
			pTex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
			pTex->setResizeNonPowerOfTwoHint(false);
			ss->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );
		}
	}

	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	ss->setMode( GL_ALPHA_TEST, osg::StateAttribute::ON );
	ss->setAttribute( new osg::AlphaFunc(osg::AlphaFunc::GREATER, 0.9f), osg::StateAttribute::ON );

	createShaders();
}

void CSulRenderInstances::createShaders()
{
	CSulShaderInstancingBillboards* p = new CSulShaderInstancingBillboards( this, m_numInstances, m_texUnit, m_texSizeSquared, m_useLights, m_min, m_max );
}

sigma::uint32 CSulRenderInstances::getNumInstances() const
{
	return m_numInstances;
}

const osg::BoundingBox&	CSulRenderInstances::getBB() const
{
	return m_bb;
}

osg::Texture2D* CSulRenderInstances::getTexturePositions() const
{
	return m_rTexturePositions;
}