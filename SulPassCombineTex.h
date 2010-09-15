// SulPassCombineTex.h

#ifndef __SULPASSCOMBINETEX_H__
#define __SULPASSCOMBINETEX_H__

#include "SulPass.h"
#include "SulShaderCombineTex.h"
#include "SulTextureBuffer.h"

class CSulPassCombineTex : public CSulPass
{
public:
	CSulPassCombineTex( osg::Texture2D* pTexIn0, osg::Texture2D* pTexIn1, osg::Texture2D* pTexOut );
	CSulPassCombineTex( CSulTextureBuffer* texBuf0, CSulTextureBuffer* texBuf1, osg::Texture2D* pTexOut );
	CSulPassCombineTex( osg::Texture2D* pTexIn0, CSulTextureBuffer* texBuf1, osg::Texture2D* pTexOut );

	CSulPassCombineTex( CSulTextureBuffer* texBuf0, CSulTextureBuffer* texBuf1 );

private:
	osg::ref_ptr<CSulShaderCombineTex> m_rShader;
};


#endif // __SULPASSCOMBINETEX_H__