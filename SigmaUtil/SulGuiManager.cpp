// SulGuiManager.cpp

#include "stdafx.h"
#include "SulGuiTextBox.h"
#include "SulGuiManager.h"
#include "SulGuiXml.h"
#include "SulGuiCompVisitor.h"
#include "SulGuiCompEditModeVisitor.h"
#include <osgDB/FileUtils>

CSulGuiManager::CSulGuiManager( osgViewer::View* pViewer, bool bFlipTopBottom )
{
	m_pParent = 0;
	m_bFlipTopBottom = bFlipTopBottom;

	m_viewW = pViewer->getCamera()->getViewport()->width();
	m_viewH = pViewer->getCamera()->getViewport()->height();
	m_rViewer = pViewer;

	m_rMT = new osg::MatrixTransform;
	m_rMT->setDataVariance( osg::Object::DYNAMIC );
	m_rMT->setMatrix( osg::Matrix::identity() );

	m_rEventHandler = new CSulGuiEventHandler;
	m_rEventHandler->setIgnoreHandledEventsMask( 0 );
	m_rEventHandler->signalViewResize.connect( this, &CSulGuiManager::onViewResize );
    pViewer->addEventHandler( m_rEventHandler );

	// create projection matrix
	setName( "CSulGuiManager -> Projection" );
	if ( bFlipTopBottom )
		setMatrix( osg::Matrix::ortho2D( 0, m_viewW, 0, m_viewH ) );
	else
		setMatrix( osg::Matrix::ortho2D( 0, m_viewW, m_viewH, 0) );

	// set reference to absolute
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	addChild( m_rMT );

	osg::StateSet* ss = m_rMT->getOrCreateStateSet();
	ss->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	ss->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	ss->setRenderBinDetails( 1000000, "DepthSortedBin" );	
}

float CSulGuiManager::getViewW()
{
	return m_viewW;
}

float CSulGuiManager::getViewH()
{
	return m_viewH;
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
		osg::notify(osg::WARN) << "ERROR: CSulGuiManager::load -> could not find xml file" << sFileXml << std::endl;
		return false;
	}

	CSulGuiXml* pXml = new CSulGuiXml( pParent?pParent:m_rMT, m_rEventHandler, m_viewW, m_viewH, rThemeXml );
	return pXml->load( fileXml );
}

void CSulGuiManager::onViewResize( float w, float h )
{
	m_viewW = w;
	m_viewH = h;

	if ( m_bFlipTopBottom )
		setMatrix( osg::Matrix::ortho2D( 0, w, 0, h) );
	else
		setMatrix( osg::Matrix::ortho2D( 0, w, h, 0) );
}

// note, in this order
//  eventTraversal
//  updateTraversal
//  renderingTraversals


void CSulGuiManager::show( bool bShow )
{
	/*
	// safety check
	if ( !getNumParents() )
		return;
		*/

	signalShow( bShow );

	m_rEventHandler->pause( !bShow );

	if ( bShow )
	{
		if ( m_pParent )
			m_pParent->addChild( this );

		m_pParent = 0;
	}
	else
	{
		if ( !m_pParent )
		{
			// safety check
			if ( !getNumParents() )
				return;

			m_pParent = getParent(0);
			m_pParent->removeChild( this );
		}
	}

	//m_rMT->setNodeMask( bShow?0xFFFFFFFF:0 );
}

bool CSulGuiManager::isVisible()
{
	return m_pParent?false:true;

//	return m_rMT->getNodeMask()==0?false:true;
}

void CSulGuiManager::setEditMode( bool bEdit )
{
	osg::ref_ptr<CSulGuiCompEditModeVisitor> r = new CSulGuiCompEditModeVisitor( bEdit );
	m_rMT->accept( *r );
}

CSulGuiComp* CSulGuiManager::get( const CSulString& id )
{
	if ( !m_rMT.valid() )
		return 0;

	osg::ref_ptr<CSulGuiCompVisitor> nv = new CSulGuiCompVisitor( id );
	nv->setNodeMaskOverride( 0xFFFFFFFF );
	m_rMT->accept( *nv );
	return nv->getFound();
}

CSulGuiTextBox* CSulGuiManager::getTextBox( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asTextBox();
	}

	return 0;
}

CSulGuiCanvas* CSulGuiManager::getCanvas( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asCanvas();
	}

	return 0;
}

CSulGuiListBox* CSulGuiManager::getListBox( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asListBox();
	}

	return 0;
}

CSulGuiButton* CSulGuiManager::getButton( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asButton();
	}

	return 0;
}

CSulGuiEditBox* CSulGuiManager::getEditBox( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asEditBox();
	}

	return 0;
}

CSulGuiRadioButton* CSulGuiManager::getRadioButton( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asRadioButton();
	}

	return 0;
}

CSulGuiComboBox* CSulGuiManager::getComboBox( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asComboBox();
	}

	return 0;
}

CSulGuiCheckBox* CSulGuiManager::getCheckBox( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asCheckBox();
	}

	return 0;
}

CSulGuiDial* CSulGuiManager::getDial( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asDial();
	}

	return 0;
}

CSulGuiProfiler* CSulGuiManager::getProfiler( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asProfiler();
	}

	return 0;
}

CSulGuiAlign* CSulGuiManager::getAlign( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asAlign();
	}

	return 0;
}

CSulGuiText* CSulGuiManager::getText( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asText();
	}

	return 0;
}

CSulGuiSliderH* CSulGuiManager::getSliderH( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asSliderH();
	}

	return 0;
}

CSulGuiTable* CSulGuiManager::getTable( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asTable();
	}

	return 0;
}

CSulGuiListView* CSulGuiManager::getListView( const CSulString& id )
{
	CSulGuiComp* p = get( id );
	if ( p )
	{
		return p->asListView();
	}

	return 0;
}
