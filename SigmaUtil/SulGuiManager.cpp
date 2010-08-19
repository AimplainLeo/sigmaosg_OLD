// SulGuiManager.cpp

#include "stdafx.h"
#include "SulGuiManager.h"
#include "SulGuiXml.h"
#include "SulGuiManagerUpdateCallback.h"
#include <osgDB/FileUtils>

CSulGuiManager::CSulGuiManager( osgViewer::View* pViewer )
{
	m_viewW = pViewer->getCamera()->getViewport()->width();
	m_viewH = pViewer->getCamera()->getViewport()->height();
	m_rViewer = pViewer;

    m_rEventHandler = new CSulGuiEventHandler;
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

	CSulGuiManagerUpdateCallback* pCB = new CSulGuiManagerUpdateCallback( pViewer );
	pCB->signalSizeChanged.connect( this, &CSulGuiManager::onViewSizeChanged );
	m_rMT->setUpdateCallback( pCB );
}

void CSulGuiManager::onViewSizeChanged( float w, float h )
{
	
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

