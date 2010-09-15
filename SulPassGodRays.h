// SulPassGodRays.h

#ifndef __SULPASSGODRAYS_H__
#define __SULPASSGODRAYS_H__

#include "SulTextureBuffer.h"
#include "SulPass.h"
#include "SulShaderGodRays.h"

class CSulPassGodRays : public CSulPass
{
public:
	CSulPassGodRays( osg::Texture2D* pTexIn, CSulTextureBuffer* pTexOut );

	CSulShaderGodRays* getShader();

private:
	osg::ref_ptr<CSulShaderGodRays> m_rShader;
};

#endif // __SULPASSGODRAYS_H__