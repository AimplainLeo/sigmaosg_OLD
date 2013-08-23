// SulNodeFinder.cpp

#include "stdafx.h"
#include "SulNodeFinder.h"

osg::Node* CSulNodeFinder::findNodeByName( osg::Node* pNode, const std::string& sName )
{
    if ( pNode->getName()==sName )
    {
        return pNode;
    }

    osg::Group* pGroup = pNode->asGroup();
    if ( pGroup )
    {
        for ( unsigned int i=0; i<pGroup->getNumChildren(); i++ )
        {
            osg::Node* pFound = findNodeByName( pGroup->getChild(i), sName );
            if ( pFound )
            {
                return pFound;
            }
        }
    }

    return 0;
}