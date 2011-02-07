// SulPassToneMap.cpp

#include "stdafx.h"
#include "SulPassToneMap.h"


class CSulFixMe : public osg::NodeCallback
{
public:
	CSulFixMe( CSulTextureBuffer* pTexLum, CSulShaderCombine* pShader )
	{
		m_rTexLum = pTexLum;
		m_rShader = pShader;
	}

    virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		traverse( node, nv );

		float aLum = 0.0f;
	}

private:
	osg::ref_ptr<CSulTextureBuffer>	m_rTexLum;
	osg::ref_ptr<CSulShaderCombine>	m_rShader;
};

class CSulFixMeDraw : public osg::Camera::DrawCallback
{
public:
	CSulFixMeDraw( osg::Image* image, CSulShaderCombine* pShader ) :
	m_rImage( image ), 
	m_LastExposure( 0.0f ),
	m_rShader( pShader )
	{
	}

	template <typename Real>
	inline Real lerp(Real x, Real y, Real t) const
	{
	  return x + t * (y - x);
	}

	virtual void operator () (osg::RenderInfo& renderInfo) const
	{
		/*
		// attempt at reading the buffer directly... failed
		osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
        int width = gc->getTraits()->width;
        int height = gc->getTraits()->height;
		osg::ref_ptr<osg::Image> image = new osg::Image;
		image->setDataType( GL_FLOAT );
		image->setInternalTextureFormat( GL_RGBA16F_ARB );
		image->readPixels(0, 0, 2, 2, GL_RGBA16F_ARB, GL_FLOAT );
		float* p = (float*)image->data();
*/
		float* p = (float*)m_rImage->data();

		osg::Vec4 c;
		osg::Vec4 w;
		float lum = 0.0f;

		for(int i=0; i<4; i++)
		{
			c.set(*p++,*p++,*p++,*p++);
			w.set(0.3, 0.59, 0.11, 0.0);
			lum += w*c;
		}
		lum *= 0.25f;

		float desiredExposure = 0.5f / lum;
		static const float adaptationSpeed = 2.0f; // 2 seconds
		float dt = renderInfo.getState()->getFrameStamp()->getReferenceTime() - m_LastFrameStamp.getReferenceTime();
		float delta = dt * adaptationSpeed;
		float exposure = lerp(m_LastExposure, desiredExposure, delta);

		m_LastExposure = exposure;
		m_LastFrameStamp = *renderInfo.getState()->getFrameStamp();

		m_rShader->setExposure( exposure );
	}

private:
	osg::ref_ptr<osg::Image> m_rImage;
	mutable osg::FrameStamp m_LastFrameStamp;
	mutable float m_LastExposure;
	osg::ref_ptr<CSulShaderCombine>	m_rShader;
};


CSulPassToneMap::CSulPassToneMap( osg::Texture2D* pTexOrg, osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, float exp, float factor, float max, CSulTexCam* pTexCamLum ) :
CSulPass( pTexIn, pTexOut, "ToneMap" )
{
	m_rShader = new CSulShaderCombine( getQuad(), exp, factor, max, pTexOrg, pTexIn );

	// if a luminance texture is given we create an update callback to handle calculations
	// for luminance and speed (auto exposure)
	if ( pTexCamLum )
	{
		//getGeode()->setUpdateCallback( new CSulFixMe( pTexLum, m_rShader ) );


		osg::Image* image = new osg::Image;
		image->allocateImage( 2, 2, 1, /*GL_RGBA16F_ARB*/GL_RGBA, GL_FLOAT );
		pTexCamLum->attach( osg::Camera::COLOR_BUFFER, image );

//		pTexCamLum->setRenderTargetImplementation( osg::Camera::SEPERATE_WINDOW );				

		pTexCamLum->setPostDrawCallback( new CSulFixMeDraw( image, m_rShader ) );
	}
}

CSulShaderCombine* CSulPassToneMap::getShader()
{
	return m_rShader;
}
