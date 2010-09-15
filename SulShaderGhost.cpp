// SulShaderGhost.cpp

#include "stdafx.h"
#include "SulShaderGhost.h"

CSulShaderGhost::CSulShaderGhost( osg::Node* pNode, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/ghost.frag" );
		addShaderVert( "shaders/ghost.vert" );
	}
}

