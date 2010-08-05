// SulGuiListBox.cpp

#include "stdafx.h"
#include "SulGuiListBox.h"
#include <osg/scissor>
#include <osgManipulator/Selection>

CSulGuiListBox::CSulGuiListBox( float x, float y, float w, float h ) :
CSulGuiCanvas( x, y, w, h )
{
	// opengl scissor?
	// clip planes
	// render to texture
	// stencil buffer
	// fixed point text positions with limits

/*
	osg::NodePath pathToRoot;
	osgManipulator::computeNodePathToRoot( *this, pathToRoot );
	osg::Matrix m = osg::computeLocalToWorld( pathToRoot );
	x = m.getTrans().x();
	y = m.getTrans().y();
*/

/*
	osg::Matrix m;
	m.setTrans( x, y, 0 );
	this->computeLocalToWorldMatrix( m, 0 );


	//osg::Scissor* pScissor = new osg::Scissor( 256+32, 512-(40+h), w, h );
	osg::Scissor* pScissor = new osg::Scissor( x, 512-(h+y), w, h );
	//osg::Scissor* pScissor = new osg::Scissor( 0, 0, 400, 512 );
	getOrCreateStateSet()->setAttributeAndModes( pScissor, osg::StateAttribute::ON );
*/
}

void CSulGuiListBox::addItem( CSulGuiItem* pItem )
{
	m_vecItem.push_back( pItem );

	// we need to set the position of the item in the listbox

	pItem->setY( m_vecItem.size()*32.0f );

	if ( pItem->getY()>getH() )
	{
		pItem->show( false );
	}
}