// SulInventoryItem.h

#ifndef __SULINVENTORYITEM_H__
#define __SULINVENTORYITEM_H__

#include "SulString.h"
#include "SulInventoryItemData.h"
#include "SulExport.h"

class SUL_EXPORT CSulInventoryItem
{
public:
	CSulInventoryItem( const CSulString& name, osg::Node* node, CSulInventoryItemData* itemData );

	osg::MatrixTransform*	getMatrixTransform();

	const CSulString&		getName();

	CSulInventoryItemData*	getData();

private:
	osg::MatrixTransform*				m_mtWheelPosition;
	CSulString							m_name;
	osg::ref_ptr<CSulInventoryItemData>	m_itemData;
};

#endif // __SULINVENTORYITEM_H__