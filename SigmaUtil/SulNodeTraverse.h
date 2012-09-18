// SulNodeTraverse.h

#ifndef __SULNODETRAVERSE_H__
#define __SULNODETRAVERSE_H__

#include "SulExport.h"

class SUL_EXPORT CSulNodeTraverse
{
public:
	static osg::Node* findNodeByName( osg::Node* pNode, const std::string& sName );
};

#endif // __SULNODETRAVERSE_H__