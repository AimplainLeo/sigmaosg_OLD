// SulRTT.h

#ifndef __SULRTT_H__
#define __SULRTT_H__

#include "SulExport.h"
#include <osg/Referenced>
#include <osg/Texture2D>

class SUL_EXPORT CSulRTT : public osg::Camera
{
public:
	enum ESETUP
	{
		SETUP_STANDARD,
		SETUP_SHADOW,
		SETUP_HDR
	};

public:
									CSulRTT( osg::Texture2D* pTex, ESETUP eSetup=SETUP_STANDARD );
									CSulRTT( float w, float h, GLint format=GL_RGBA, GLenum source_type=GL_UNSIGNED_BYTE );
									CSulRTT( float w, float h, ESETUP eSetup );

	osg::Texture2D*					getTexture();

	// specialize methods
	void							setupStandard();
	void							setupShadow();

private:
	osg::ref_ptr<osg::Texture2D>	m_rTex;

	float							m_w;
	float							m_h;
	GLint							m_format;
	GLenum							m_source_type;
};

#endif // __SULRTT_H__