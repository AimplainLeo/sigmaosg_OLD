// SulNode.cpp

#include "stdafx.h"
#include "SulNode.h"
#include "SulNodeGroup.h"

CSulNode::CSulNode() 
{
	m_parent = 0;
}

void CSulNode::setParent( CSulNodeGroup* parent )
{
	m_parent = parent;
}

CSulNodeGroup* CSulNode::getParent()
{
	return m_parent;
}