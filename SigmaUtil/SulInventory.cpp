// SulInventory.cpp

#include "stdafx.h"
#include "SulInventory.h"
#include "SulInventoryUpdateCallback.h"

CSulInventory::CSulInventory( CSulInventory* parent ) :
m_dist( 1.0f ),
m_iTargetIndex(0),
m_fWheelRotAng(0.0f),
m_iRotDir(0),
m_iCurrentIndex(0),
m_distFactor(1.0)
{
	m_inventoryParent = parent;

	addUpdateCallback( new CSulInventoryUpdateCallback( this ) );

	m_pos.set( 0.0f, 3.0f, -0.5f );

	osg::Matrix m;
	m.makeTranslate( m_pos );
	setMatrix( m );

	setNodeMask( 0 );
}

CSulInventory* CSulInventory::getParentInventory()
{
	return m_inventoryParent;
}

void CSulInventory::setDistanceFactor( double factor )
{
	m_distFactor = factor;
	calcWheel();
}

void CSulInventory::show()
{
	setNodeMask( 0xFFFFFFFF );
}

void CSulInventory::hide()
{
	setNodeMask( 0 );
}

void CSulInventory::toggle()
{
	setNodeMask( getNodeMask()?0:0xFFFFFFFF );
}

bool CSulInventory::isVisible()
{
	return getNodeMask()?true:false;
}

sigma::int32 CSulInventory::count()
{
	return m_vecItems.size();
}

void CSulInventory::rotateRight()
{
	// already rotating
	if ( m_iRotDir==1 )
	{
		m_iTargetIndex++;
		return;
	}

	m_iRotDir = 1;
	m_iTargetIndex = 1;
}

void CSulInventory::rotateLeft()
{
	// already rotating
	if ( m_iRotDir==-1 )
	{
		m_iTargetIndex++;
		return;
	}

	m_iRotDir = -1;
	m_iTargetIndex = 1;
}

void CSulInventory::add( CSulInventoryItem* item )
{
	addChild( item->getMatrixTransform() );
	m_vecItems.push_back( item );
	calcWheel();
}

void CSulInventory::remove( CSulInventoryItem* item )
{
	removeChild( item->getMatrixTransform() );

	VEC_ITEMS::iterator iFound = std::find( m_vecItems.begin(), m_vecItems.end(), item );

	m_vecItems.erase( iFound );

	calcWheel();
}

void CSulInventory::calcWheel()
{
	// special case
	if ( m_vecItems.size()==0 )
		return;

	if ( m_vecItems.size()==1 )
	{
		osg::MatrixTransform* mt = m_vecItems[0]->getMatrixTransform();
		osg::Matrix m;
		m.makeTranslate( 0, 0, 0 );
		mt->setMatrix( m );
		return;
	}

	float f = (2.0f*osg::PI)/(float)m_vecItems.size();

	int iCount = 0;

	VEC_ITEMS::iterator i = m_vecItems.begin();
	VEC_ITEMS::iterator ie = m_vecItems.end();

	while ( i!=ie )
	{
		float x = cos( osg::PI_2+(float)iCount*f )*m_dist*m_distFactor;
		float y = sin( -osg::PI_2+(float)iCount*f )*m_dist*m_distFactor;

		osg::MatrixTransform* mt = (*i)->getMatrixTransform();
		osg::Matrix m;
		m.makeTranslate( x, y, 0.0f );
		mt->setMatrix( m );

		++iCount;
		++i;
	}
}

void CSulInventory::calcItemAtAngle( double a )
{
	// calc angle between items (ii)
	double i = (double)m_vecItems.size();
	double ii = (2.0*osg::PI)/i;

	// adjust "a" with half angle of ii (this will create the effect of equal spacing on each side of the item)
	a += ii/2.0;

	// normalize a so it's between 0 and 2*PI
	int c = a/(2.0*osg::PI);
	double b = a-(2.0*osg::PI*(double)c);

	// check for negative rotation values
	b = b<0.0f?2.0*osg::PI+b:b;

	// set the correct text for item
	int item_val = b/ii;

	// update when new item starts to come infront
	if ( item_val!=m_iCurrentIndex && m_iTargetIndex )
	{
		m_iCurrentIndex = item_val;
		m_iTargetIndex--;
		
		if ( m_iRotDir==1 )
		{
			double tmp = b - ((2.0*osg::PI)/i)*item_val;
			aa = b-tmp;
		}
		else
		{
			double tmp = b - ((2.0*osg::PI)/i)*item_val;
			aa = b-tmp;
		}
	}

	// stop first when item is very close to the middle infront
	if ( !m_iTargetIndex && m_iRotDir!=0 )
	{
		if ( m_iRotDir>0 && (b-aa)>=(ii/2.0f) )
		{
			m_iRotDir = 0;
			snap();
		}

		// b is decreasing
		if ( m_iRotDir<0 && (b-aa)<=(ii/2.0f) )
		{
			m_iRotDir = 0;
			snap();
		}
	}
}

void CSulInventory::snap()
{
	// rotate wheel to exact rotation for m_iCurrentIndex
	double i = (double)m_vecItems.size();
	m_fWheelRotAng = (float)(((2.0*osg::PI)/i)*m_iCurrentIndex);
	osg::Quat q( m_fWheelRotAng, osg::Vec3(0,0,1) );

	// rotate the wheel
	osg::Matrix m;
	m.makeTranslate( m_pos );
	m = osg::Matrix(q)*m;
	setMatrix( m );
}

const CSulString& CSulInventory::getCurrentItemName()
{
	return m_vecItems[m_iCurrentIndex]->getName();
}

CSulInventoryItem* CSulInventory::getCurrentItem()
{
	return m_vecItems[m_iCurrentIndex];
}


void CSulInventory::update( double dt )
{
	if ( dt>0.1 ) dt=0.1;

	double speed = (m_iTargetIndex+1.0)*2.0;		// increase speed if user has done a lot of scrolling with the mouse
	m_fWheelRotAng += dt*speed*(float)m_iRotDir;

	osg::Quat q( m_fWheelRotAng, osg::Vec3(0,0,1) );

	// rotate the wheel
	osg::Matrix m;
	m.makeTranslate( m_pos );
	m = osg::Matrix(q)*m;
	setMatrix( m );

	// update text for item infront
	calcItemAtAngle( m_fWheelRotAng );
}
