// SulGuiButton.cpp

#include "stdafx.h"
#include "SulGuiButton.h"
#include "SulGuiText.h"

CSulGuiButton::CSulGuiButton( const CSulString& sName, float x, float y, float w, float h ) :
CSulGuiComp( sName, x, y, w!=0.0?w:128.0f, h!=0.0f?h:32.0f )
{
	addEvents( EVENT_MOUSE_MOVE );	
	addEvents( EVENT_MOUSE_PUSHED );	
	addEvents( EVENT_MOUSE_RELEASED );

	setQuadColor( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );

	test = new CSulGuiText( "Hello World", 0, 0, 100, 100, 16 );
	addChild( test );
}


#include <osgManipulator/Selection>

void CSulGuiButton::eventMouseMove( float mouse_x, float mouse_y )
{
/*
	osg::Matrix m;
	m.setTrans( mouse_x, mouse_y, 0 );
	computeWorldToLocalMatrix( m, 0 );
	mouse_x = m.getTrans().x();
*/

    osg::NodePath pathToRoot;
    osgManipulator::computeNodePathToRoot(*this,pathToRoot);
	osg::Matrix m = osg::computeLocalToWorld( pathToRoot );

/*
	osg::Matrix m;
	computeLocalToWorldMatrix( m, 0 );
*/
	mouse_x = m.getTrans().x();

	CSulString s;
	s.Format( "x= %f, y= %f", mouse_x, mouse_y );
	test->setName( s );
}