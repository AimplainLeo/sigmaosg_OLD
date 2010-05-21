// SulShaderNormalMapObjectSpace.cpp

#include "stdafx.h"
#include "SulShaderNormalMapObjectSpace.h"
#include <osg/geode>
#include <osg/geometry>

CSulShaderNormalMapObjectSpace::CSulShaderNormalMapObjectSpace( osg::Node* pNode, Sigma::int32 indexLight, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/normalmap_objectspace.frag" );
		addShaderVert( "shaders/normalmap_objectspace.vert" );

		osg::Uniform* uniformIndexLight = new osg::Uniform( osg::Uniform::INT, "indexLight" );
		uniformIndexLight->set( indexLight );
		m_rSS->addUniform( uniformIndexLight );

		osg::Uniform* uniformColorMap = new osg::Uniform( osg::Uniform::SAMPLER_2D, "colorMap" );
		uniformColorMap->set( 0 );		
		m_rSS->addUniform( uniformColorMap );

		osg::Uniform* uniformNormalMap = new osg::Uniform( osg::Uniform::SAMPLER_2D, "normalMap" );
		uniformNormalMap->set( 1 );		
		m_rSS->addUniform( uniformNormalMap );
	}
}