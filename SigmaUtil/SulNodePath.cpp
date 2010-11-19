// SulNodePath.cpp

#include "stdafx.h"
#include "SulNodePath.h"

void sulNodePath( osg::Node& node, osg::NodePath& np, osg::Node* haltTraversalAtNode, bool suppressWarning )
{
    np.clear();
    
    osg::NodePathList nodePaths = node.getParentalNodePaths( haltTraversalAtNode );
    
    if (!nodePaths.empty())
    {
        np = nodePaths.front();
        if ( !suppressWarning && nodePaths.size()>1 )
        {
			osg::notify(osg::NOTICE) << "WARNING: sulNodePath() - taking first parent path, ignoring others." << std::endl;
        }
    }
}
