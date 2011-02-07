// SulPassCombineTex.cpp

#include "stdafx.h"
#include "SulPassCombineTex.h"

CSulPassCombineTex::CSulPassCombineTex( osg::Texture2D* pTexIn0, osg::Texture2D* pTexIn1, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn0, pTexOut, "CombineTex" )
{
	m_rShader = new CSulShaderCombineTex( getQuad(), pTexIn0, pTexIn1 );
}

CSulPassCombineTex::CSulPassCombineTex( CSulTextureBuffer* texBuf0, CSulTextureBuffer* texBuf1, osg::Texture2D* pTexOut ) :
CSulPass( texBuf0->getOut(), pTexOut, "CombineTex" )
{
	m_rShader = new CSulShaderCombineTex( getQuad(), texBuf0->getOut(), texBuf1->getOut() );
}

CSulPassCombineTex::CSulPassCombineTex( osg::Texture2D* pTexIn0, CSulTextureBuffer* texBuf1, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn0, pTexOut, "CombineTex" )
{
	m_rShader = new CSulShaderCombineTex( getQuad(), pTexIn0, texBuf1->getOut() );
}

CSulPassCombineTex::CSulPassCombineTex( CSulTextureBuffer* texBuf0, CSulTextureBuffer* texBuf1 ) :
CSulPass( texBuf0->getOut(), texBuf0->getIn(), "CombineTex" )
{
	m_rShader = new CSulShaderCombineTex( getQuad(), texBuf0->getOut(), texBuf1->getOut() );
	texBuf0->swap();		
}
