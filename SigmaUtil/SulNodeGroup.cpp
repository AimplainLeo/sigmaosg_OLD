// SulNodeGroup.cpp

#include "stdafx.h"
#include "SulNodeGroup.h"

CSulNodeGroup::CSulNodeGroup() :
CSulNode()
{
}

void CSulNodeGroup::addChild( CSulNode* node )
{
	m_children.push_back( node );
	node->setParent( this );
}

const CSulNode::VEC_NODE& CSulNodeGroup::getChildList()
{
	return m_children;
}