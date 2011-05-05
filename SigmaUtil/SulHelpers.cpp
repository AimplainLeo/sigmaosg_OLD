// SulHelpers.cpp

#include "stdafx.h"
#include "SulHelpers.h"
#include "SulTypes.h"

bool CSulHelpers::isNodeInGroup( const osg::Node* pNode, const osg::Group* pGroup )
{
	for ( sigma::uint32 i=0; i<pGroup->getNumChildren(); i++ )
	{
		if ( pGroup->getChild( i )==pNode )
			return true;
	}

	return false;
}