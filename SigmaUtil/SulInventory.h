// SulInventory.h

#ifndef __SULINVENTORY_H__
#define __SULINVENTORY_H__

#include "SulInventoryItem.h"
#include "SulExport.h"

class SUL_EXPORT CSulInventory : public osg::MatrixTransform
{
private:
	friend class CSulInventoryUpdateCallback;

public:
					CSulInventory( CSulInventory* parent );

	CSulInventory*	getParentInventory();

	void			setDistanceFactor( double factor );

	void			show();
	void			hide();
	void			toggle();
	bool			isVisible();

	void			rotateRight();
	void			rotateLeft();

	void			add( CSulInventoryItem* item );
	void			remove( CSulInventoryItem* item );

	sigma::int32	count();

	const CSulString&		getCurrentItemName();
	CSulInventoryItem*		getCurrentItem();

protected:
	void	update( double dt );

private:
	void	calcWheel();
	void	calcItemAtAngle( double a );
	void	snap();

private:
	float										m_dist;		// item distance from center of the wheel
	typedef std::vector< CSulInventoryItem* >	VEC_ITEMS;
	VEC_ITEMS									m_vecItems;
	sigma::int32								m_iTargetIndex;
	float										m_fWheelRotAng;
	sigma::int32								m_iRotDir;
	sigma::int32								m_iCurrentIndex;
	osg::Vec3									m_pos;
	double										m_distFactor;
	CSulInventory*								m_inventoryParent;

	double aa;
};

#endif // __SULINVENTORY_H__