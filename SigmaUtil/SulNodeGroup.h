// SulNodeGroup.h

#ifndef __SULNODEGROUP_H__
#define __SULNODEGROUP_H__

#include "SulNode.h"

class CSulNodeGroup : public CSulNode
{
public:
	CSulNodeGroup();

	void addChild( CSulNode* node );

	const CSulNode::VEC_NODE& getChildList();

private:
	CSulNode::VEC_NODE	m_children;	
};

#endif // __SULNODEGROUP_H__