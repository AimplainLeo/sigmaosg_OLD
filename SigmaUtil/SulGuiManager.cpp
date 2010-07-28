// SulGuiManager.cpp

#include "stdafx.h"
#include "SulGuiManager.h"
#include "SulGuiXml.h"

CSulGuiManager::CSulGuiManager( osgViewer::Viewer* pViewer, float viewW, float viewH )
{
	m_rViewer = pViewer;

    m_rEventHandler = new CSulGuiEventHandler;
    pViewer->addEventHandler( m_rEventHandler );

	m_rMT = new osg::MatrixTransform;
	m_rMT->setMatrix( osg::Matrix::identity() );

	// create projection matrix
	setName( "CSulGuiManager -> Projection" );
	setMatrix( osg::Matrix::ortho2D( 0, viewW, viewH, 0) );
	//setMatrix( osg::Matrix::ortho2D( 0, viewW, 0, viewH ) );

	// set reference to absolute
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	addChild( m_rMT );

	osg::StateSet* ss = m_rMT->getOrCreateStateSet();
	ss->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
}

bool CSulGuiManager::load( const CSulString& sFileXml, osg::Group* pParent )
{
	CSulGuiXml* pXml = new CSulGuiXml( pParent?pParent:m_rMT, m_rEventHandler );
	return pXml->load( sFileXml );
}