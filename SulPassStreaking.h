// SulPassStreaking.h

#ifndef __SULPASSSTREAKING_H__
#define __SULPASSSTREAKING_H__

#include "SulShaderStreak.h"
#include "SulPass.h"
#include "SulTextureBuffer.h"

class CSulPassStreaking : public CSulPass
{
public:
	CSulPassStreaking( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, float offsetFactor, float glowFactor );
	CSulPassStreaking( CSulTextureBuffer* texBuf, float offsetFactor, float glowFactor );

	CSulShaderStreak* getShader();

	void setGlowFactor( float glowFactor );

private:
	osg::ref_ptr<CSulShaderStreak> m_rShader;
};

#endif // __SULPASSSTREAKING_H__