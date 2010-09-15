// SulNodeTraverse.cpp

#include "stdafx.h"
#include "SulNodeTraverse.h"

osg::Node* CSulNodeTraverse::FindNodeByName( osg::Node* pNode, const std::string& sName )
{
	if ( !pNode )
	{
		return 0;
	}

    if ( pNode->getName()==sName )
    {
        return pNode;
    }

    osg::Group* pGroup = pNode->asGroup();
    if ( pGroup )
    {
        for ( unsigned int i=0; i<pGroup->getNumChildren(); i++ )
        {
            osg::Node* pFound = FindNodeByName( pGroup->getChild(i), sName );
            if ( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}
