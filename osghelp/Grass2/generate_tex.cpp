// generate_tex.cpp

#include "stdafx.h"
#include "generate_tex.h"
#include <iostream>
#include <osgdb/writefile>
#include <assert.h>

class CGenUpdateCallback : public osg::NodeCallback
{
public:
	CGenUpdateCallback( CGenerateTex* pGenTex, osg::Image* pImage )
	{
		m_rImage = pImage;
		m_pGenTex = pGenTex;
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		static int count = 0;
		count++;
/*
		if ( count==19 )
		{
			float f0 = ((float*)m_rImage->data( 0, 0 ))[0];
			std::cout << "top-left value = " << f0 << std::endl;

			float f1 = ((float*)m_rImage->data( 512, 512 ))[0];
			std::cout << "center value = " << f1 << std::endl;

			FILE* file = fopen( "myass.raw", "w+" );
			assert( file );

			float* p = (float*)m_rImage->data( 0, 0 );
			fwrite( p, 1024*1024*4, 1, file );
			fclose( file );
		}
*/

		// trying to create the png file automatically using the m_rImageData
		if ( count==20 )
		{
			bool bRet = osgDB::writeImageFile( *m_rImage, "myass.tif");
			if ( !bRet )
			{
				std::cout << "writeImageFile FAILED" << std::endl;
			}
		}

		// traverse subtree
        traverse(node,nv);
    }

private:
	CGenerateTex* m_pGenTex;
	osg::ref_ptr<osg::Image>	m_rImage;
};


void CGenerateTex::create( 
	osg::Node* pRenderMe,			// this will be rendered
	osg::Group* pRootGroup,			// need to add generator to scene (so it will work)
	float x,						// center x location of scene (also center location for texture)
	float y,						// center x location of scene (also center location for texture)
	unsigned int	tXY,			// texture size squared
	unsigned int	sXY				// screen size squared
	)
{
	m_rRTT = new CSulRTT( tXY, tXY, GL_LUMINANCE, GL_FLOAT ); 
	m_rRTT->getTexture()->setInternalFormat( GL_LUMINANCE32F_ARB );
	m_rRTT->getTexture()->setSourceType( GL_FLOAT );
	m_rRTT->getTexture()->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	m_rRTT->getTexture()->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
	m_rRTT->getCamera()->setClearColor( osg::Vec4(0,0,0,1) );
	pRootGroup->addChild( m_rRTT->getCamera() );

	// need to ortho camera
	float l = sXY/2.0f;
	float r = -l;
	float b = l;
	float t = -l;
	m_rRTT->getCamera()->setProjectionMatrixAsOrtho( l, r, b, t, 0.1f, 1000000.0f  );

	// RTT camera should follow main camera
	osg::Matrix mViewMatrix;
	float m[16] =
	{
		 0, 1, 0, 0,
		-1, 0, 0, 0,
		 0, 0, 1, 0,
		 0, 0,-10000, 1
	};
	mViewMatrix.set( m );
	m_rRTT->getCamera()->setViewMatrix( mViewMatrix );

	////////////////////////////////////////////////////////////////////
	// shader to make the render height instead of texture
	////////////////////////////////////////////////////////////////////

	osg::Group* pGroupRenderMe = new osg::Group;
	pGroupRenderMe->addChild( pRenderMe );
	getCamera()->addChild( pGroupRenderMe );

	osg::StateSet* ss = pGroupRenderMe->getOrCreateStateSet();

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->loadShaderSourceFromFile( "height_render.vert" );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    fragShader->loadShaderSourceFromFile( "height_render.frag" );

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
	program->addShader( fragShader.get() );

	osg::Uniform* uniformInverseViewMatrix = new osg::Uniform( "viewInverseMatrixCam", m_rRTT->getCamera()->getInverseViewMatrix() );
    ss->addUniform( uniformInverseViewMatrix );

	ss->setAttribute( program.get(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );


	m_rImage = new osg::Image;
	//m_rImage->allocateImage( 1024, 1024, 1, GL_LUMINANCE, GL_BYTE );		// fails
	//m_rImage->allocateImage( 1024, 1024, 8, GL_LUMINANCE, GL_BYTE );		// fails
	//m_rImage->allocateImage( 1024, 1024, 24, GL_LUMINANCE, GL_BYTE );		// fails


//	m_rRTT->getCamera()->setRenderTargetImplementation( osg::Camera::PIXEL_BUFFER );	// this will render only to the texture
	//m_rImage->allocateImage( 1024, 1024, 16, GL_LUMINANCE, GL_FLOAT );	// fails
	m_rImage->allocateImage( 1024, 1024, 1,  GL_LUMINANCE, GL_FLOAT );	// fails (there is a center value,.. but no image on disk)
	//m_rImage->allocateImage( 1024, 1024, 8,  GL_LUMINANCE, GL_FLOAT );	// fails (there is a center value,.. but no image on disk)
	//m_rImage->allocateImage( 1024, 1024, 32, GL_LUMINANCE, GL_FLOAT );	// fails
	//m_rImage->allocateImage( 1024, 1024, 24, GL_LUMINANCE, GL_4_BYTES );	// fails with crash
	//m_rImage->allocateImage( 1024, 1024, 24, GL_RGB, GL_BYTE );			// works ( image 8bit in each RGB )
	//m_rImage->allocateImage( 1024, 1024, 24, GL_RGB, GL_FLOAT );			// works ( image 8bit in each RGB ), but all values zero
	//m_rImage->allocateImage( 1024, 1024, 24, GL_RGB, GL_4_BYTES );		// fails with crash
	//m_rImage->allocateImage( 1024, 1024, 32, GL_RED, GL_FLOAT );			// works, but fails when writing image to file
	//m_rImage->allocateImage( 1024, 1024, 1,  GL_ALPHA, GL_FLOAT );			// hmmm, weird alternating values on disk image
	//m_rImage->allocateImage( 1024, 1024, 32,  GL_ALPHA, GL_FLOAT );			// hmmm, weird alternating values on disk image
	//m_rImage->allocateImage( 1024, 1024, 1,  GL_RGBA, GL_FLOAT );				// hmmm, weird checker pattern on disk image

	m_rImage->setInternalTextureFormat( GL_LUMINANCE32F_ARB );

//	m_rRTT->getCamera()->detach( osg::Camera::COLOR_BUFFER );
	m_rRTT->getCamera()->attach( osg::Camera::COLOR_BUFFER, m_rImage );
	pGroupRenderMe->setUpdateCallback( new CGenUpdateCallback( this, m_rImage ) );

//	pGroupRenderMe->setUpdateCallback( new CGenUpdateCallback( this, 0 ) );
}

osg::Camera* CGenerateTex::getCamera()
{
	return m_rRTT->getCamera();
}

osg::Texture2D*	CGenerateTex::getTexture()
{
	return m_rRTT->getTexture();
}
