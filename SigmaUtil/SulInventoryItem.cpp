// SulInventoryItem.cpp

#include "stdafx.h"
#include "SulInventoryItem.h"

CSulInventoryItem::CSulInventoryItem( const CSulString& name, osg::Node* node, CSulInventoryItemData* itemData )
{
	m_name = name;

	m_mtWheelPosition = new osg::MatrixTransform;
	m_mtWheelPosition->addChild( node );

	m_itemData = itemData;
}

osg::MatrixTransform* CSulInventoryItem::getMatrixTransform()
{
	return m_mtWheelPosition;
}

const CSulString& CSulInventoryItem::getName()
{
	return m_name;
}

CSulInventoryItemData* CSulInventoryItem::getData()
{
	return m_itemData;
}