// SulShaderZ.cpp

#include "stdafx.h"
#include "SulShaderZ.h"

CSulShaderZ::CSulShaderZ( osg::Node* pNode, osg::Program* pProg ) :
CSulShaderBase( pNode,pProg )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/z.frag" );
		addShaderVert( "shaders/z.vert" );
	}
}
