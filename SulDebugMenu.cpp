// SulDebugMenu.cpp

#include "stdafx.h"
#include "SulDebugMenu.h"
#include <osgText/text>

CSulDebugMenu::CSulDebugMenu() :
osg::Group(),
m_iCurSel( 0 )
{
	osg::Camera* camera = new osg::Camera;
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setProjectionMatrixAsOrtho2D(0,800,0,600);
	camera->setViewMatrix(osg::Matrix::identity());
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
	addChild( camera );

	m_geode = new osg::Geode;
	camera->addChild( m_geode );
}

void CSulDebugMenu::add( CSulDebugValueBase* pVal )
{
	m_vecVal.push_back( pVal );
	int i = m_vecVal.size();

	if ( i==1 )
	{
		pVal->setTextColor( osg::Vec4(1.0f,1.0f,1.0f,1.0f) );
	}

	m_geode->addDrawable( pVal->getTextTitleObj() );
	m_geode->addDrawable( pVal->getTextValueObj() );

	pVal->setTextPosition( 16.0f, 600.0f-(16.0f*i) );
}

std::string CSulDebugMenu::getCurTitle()
{
	return m_vecVal[m_iCurSel]->getTitle();
}

CSulDebugValueBase*	CSulDebugMenu::getCurValueObj()
{
	return m_vecVal[m_iCurSel];
}

CSulDebugValueBase*	CSulDebugMenu::getValueObj( const std::string& sTitle )
{
	VECTOR_VAL::iterator i;

	i = m_vecVal.begin();
	while ( i!=m_vecVal.end() )
	{
		if ( (*i)->getTitle()==sTitle )
		{
			return (*i);
		}

		i++;
	}

	return 0;
}

void CSulDebugMenu::next()
{
	m_vecVal[m_iCurSel]->setTextColor( osg::Vec4(1.0f,1.0f,0.0f,1.0f) );

	m_iCurSel++;
	if ( m_iCurSel>=(int)m_vecVal.size() )
	{
		m_iCurSel = 0;
	}

	m_vecVal[m_iCurSel]->setTextColor( osg::Vec4(1.0f,1.0f,1.0f,1.0f) );
}

void CSulDebugMenu::prev()
{
	m_vecVal[m_iCurSel]->setTextColor( osg::Vec4(1.0f,1.0f,0.0f,1.0f) );

	m_iCurSel--;
	if ( m_iCurSel<0 )
	{
		m_iCurSel = m_vecVal.size()-1;
	}

	m_vecVal[m_iCurSel]->setTextColor( osg::Vec4(1.0f,1.0f,1.0f,1.0f) );
}

void CSulDebugMenu::inc()
{
	m_vecVal[m_iCurSel]->inc();
	valueChanged( m_vecVal[m_iCurSel] );
}

void CSulDebugMenu::dec()
{
	m_vecVal[m_iCurSel]->dec();
	valueChanged( m_vecVal[m_iCurSel] );
}

void CSulDebugMenu::valueChanged( CSulDebugValueBase* pObj )
{
}


