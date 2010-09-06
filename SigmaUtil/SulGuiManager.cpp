// SulGuiManager.cpp

#include "stdafx.h"
#include "SulGuiManager.h"
#include "SulGuiXml.h"
#include "SulGuiCompVisitor.h"
#include "SulGuiCompEditModeVisitor.h"
#include <osgDB/FileUtils>

CSulGuiManager::CSulGuiManager( osgViewer::View* pViewer )
{
	m_viewW = pViewer->getCamera()->getViewport()->width();
	m_viewH = pViewer->getCamera()->getViewport()->height();
	m_rViewer = pViewer;

	m_rEventHandler = new CSulGuiEventHandler;
	//m_rEventHandler->setIgnoreHandledEventsMask( osgGA::GUIEventAdapter::PUSH | osgGA::GUIEventAdapter::MOVE | osgGA::GUIEventAdapter::DRAG | osgGA::GUIEventAdapter::RELEASE | osgGA::GUIEventAdapter::RESIZE | osgGA::GUIEventAdapter::KEYDOWN);
	m_rEventHandler->setIgnoreHandledEventsMask( 0 );
	m_rEventHandler->signalViewResize.connect( this, &CSulGuiManager::onViewResize );
    pViewer->addEventHandler( m_rEventHandler );

	m_rMT = new osg::MatrixTransform;
	m_rMT->setMatrix( osg::Matrix::identity() );

	// create projection matrix
	setName( "CSulGuiManager -> Projection" );
	setMatrix( osg::Matrix::ortho2D( 0, m_viewW, m_viewH, 0) );

	// set reference to absolute
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	addChild( m_rMT );

	osg::StateSet* ss = m_rMT->getOrCreateStateSet();
	ss->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
}

bool CSulGuiManager::load( const CSulString& sFileXml, osg::Group* pParent, CSulString sFileThemeXml )
{
	osg::ref_ptr<CSulGuiThemeXml> rThemeXml;
	if ( !sFileThemeXml.empty() )
	{
		std::string fileTheme = osgDB::findDataFile( sFileThemeXml );
		if ( fileTheme.empty() )
		{
			osg::notify(osg::WARN) << "ERROR: CSulGuiManager::load -> could not find them file" << sFileThemeXml << std::endl;
		}
		else
		{
			rThemeXml = new CSulGuiThemeXml;
			if ( !rThemeXml->load( fileTheme ) )
			{
				osg::notify(osg::WARN) << "WARNING: CSulGuiManager::load -> could not load theme [" << sFileThemeXml << "]" << std::endl;
			}
		}
	}

	std::string fileXml = osgDB::findDataFile( sFileXml );
	if ( fileXml.empty() )
	{
		osg::notify(osg::WARN) << "ERROR: CSulGuiManager::load -> could not find xml file" << fileXml << std::endl;
		return false;
	}

	CSulGuiXml* pXml = new CSulGuiXml( pParent?pParent:m_rMT, m_rEventHandler, m_viewW, m_viewH, rThemeXml );
	return pXml->load( fileXml );
}

void CSulGuiManager::onViewResize( float w, float h )
{
	setMatrix( osg::Matrix::ortho2D( 0, w, h, 0) );
}

void CSulGuiManager::show( bool bShow )
{
	m_rMT->setNodeMask( bShow?0xFFFFFFFF:0 );
}

void CSulGuiManager::setEditMode( bool bEdit )
{
	osg::ref_ptr<CSulGuiCompEditModeVisitor> r = new CSulGuiCompEditModeVisitor( bEdit );
	m_rMT->accept( *r );
}

CSulGuiComp* CSulGuiManager::get( const CSulString& id )
{
	osg::ref_ptr<CSulGuiCompVisitor> nv = new CSulGuiCompVisitor( id );
	nv->setNodeMaskOverride( 0xFFFFFFFF );
	m_rMT->accept( *nv );
	return nv->getFound();
}

