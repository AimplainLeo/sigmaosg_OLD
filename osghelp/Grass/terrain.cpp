// terrain.cpp

#include "stdafx.h"
#include "terrain.h"
#include <osgDB/ReadFile>

class CMyUpdateCallback : public osg::NodeCallback
{
public:
	CMyUpdateCallback( osg::Camera* pCam, osg::Uniform* uniformViewMat, osg::Uniform* uniformProjMat )
	{
		m_rCam = pCam;
		m_rViewMat = uniformViewMat;
		m_rProjMat = uniformProjMat;
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		m_rViewMat->set( m_rCam->getViewMatrix() );
		m_rProjMat->set( m_rCam->getProjectionMatrix() );

        // traverse subtree
        traverse(node,nv);
    }

private:
	osg::ref_ptr<osg::Camera>	m_rCam;
	osg::ref_ptr<osg::Uniform>	m_rViewMat;
	osg::ref_ptr<osg::Uniform>	m_rProjMat;
};

void CTerrain::Init()
{
	rTerrain = osgDB::readNodeFile("terrain_simple.ive");
	rTerrain->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	rTerrainShader = new osg::Group;
	rTerrainShader->addChild( rTerrain );
}

osg::Node* CTerrain::getNode()
{
	return rTerrain;
}

osg::Node* CTerrain::getNodeWithShader()
{
	return rTerrainShader;	
}

void CTerrain::setCam( osg::Camera* pCam )
{
	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->loadShaderSourceFromFile( "terrain.vert" );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
   fragShader->loadShaderSourceFromFile( "terrain.frag" );

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
	program->addShader( fragShader.get() );

	osg::StateSet* ss = rTerrainShader->getOrCreateStateSet();
	ss->setAttribute( program.get(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

	osg::Matrix viewMat = pCam->getViewMatrix();
	osg::Uniform* uniformViewMat = new osg::Uniform( "viewMat", viewMat );
    ss->addUniform( uniformViewMat );

	osg::Matrix projMat = pCam->getProjectionMatrix();
	osg::Uniform* uniformProjMat = new osg::Uniform( "projMat", projMat );
    ss->addUniform( uniformProjMat );

	rTerrainShader->setUpdateCallback( new CMyUpdateCallback( pCam, uniformViewMat, uniformProjMat ) );
}
