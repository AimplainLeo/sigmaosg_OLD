// SulPassNormal.cpp

#include "stdafx.h"
#include "SulPassNormal.h"

CSulPassNormal::CSulPassNormal( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut ) :
CSulPass( pTexIn, pTexOut, "Normal" )
{
}

CSulPassNormal::CSulPassNormal( osg::Texture2D* pTexIn, CSulTextureBuffer* pTexOut ) :
CSulPass( pTexIn, pTexOut->getIn(), "Normal" )
{
}

CSulPassNormal::CSulPassNormal(CSulTextureBuffer* pTexIn, CSulTextureBuffer* pTexOut ) :
CSulPass( pTexIn->getOut(), pTexOut->getIn(), "Normal" )
{
}