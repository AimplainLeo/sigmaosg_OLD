// SulHelpers.h

#ifndef __SULHELPERS_H__
#define __SULHELPERS_H__

#include "SulExport.h"

class SUL_EXPORT CSulHelpers
{
public:
	static bool isNodeInGroup( const osg::Node* pNode, const osg::Group* pGroup );
};

#endif // __SULHELPERS_H__