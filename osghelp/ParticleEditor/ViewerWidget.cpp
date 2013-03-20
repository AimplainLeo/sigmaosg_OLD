// ViewerWidget.cpp

#include "stdafx.h"
#include "ViewerWidget.moc"
#include "ViewerWidget.h"
#include "ParticleTrackballManipulator.h"
#include <SigmaUtil/SulParticleSystemContainerOsg.h>
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <osgViewer/ViewerEventHandlers>
#include <osgQt/GraphicsWindowQt>
#include <QtGui/qfiledialog>

CViewerWidget::CViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
	m_group = new osg::Group;

    setThreadingModel(threadingModel);

	QGridLayout* grid = new QGridLayout;
	setLayout( grid );

	// FIXME: don't know why the buttons do not align to the left?! what am I missing
	// create buttons
	QHBoxLayout* layoutButtons = new QHBoxLayout;
	QWidget* widgetButtons = new QWidget;
	widgetButtons->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed ); 
	widgetButtons->setLayout( layoutButtons );
	grid->addWidget( widgetButtons, 0, 0 );

	QPushButton* load = new QPushButton( "Load" );
	load->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed ); 
	layoutButtons->addWidget( load, Qt::AlignLeft );	
	connect( load, SIGNAL(clicked()), this, SLOT(load()) );

	QPushButton* save = new QPushButton( "Save" );
	save->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed ); 
	layoutButtons->addWidget( save, Qt::AlignLeft );	
	connect( save, SIGNAL(clicked()), this, SLOT(save()) );


	m_particleSystem = new CSulParticleSystemOsg;
	m_group->addChild( m_particleSystem );

	// create 3D view
	QWidget* widgetView3D = addViewWidget( createCamera( 0, 0, 512, 512 ), createScene() );

	// create property sheet
	QWidget* widgetPropertySheet = m_particleSystem->createPropertySheet();

	// create scroll area
	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	scrollArea->setWidget( widgetPropertySheet );
	scrollArea->setWidgetResizable( true );

	// splitter
	QSplitter* splitter = new QSplitter;
	splitter->addWidget( widgetView3D );
	splitter->addWidget( scrollArea );
	grid->addWidget( splitter, 1,0 );

	// we need a timer to do the osg update
	connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
    _timer.start( 10 );
}
    
QWidget* CViewerWidget::addViewWidget( osg::Camera* camera, osg::Node* scene )
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera( camera );
    addView( view );
        
    view->setSceneData( scene );
    view->addEventHandler( new osgViewer::StatsHandler );

	CParticleTrackballManipulator* tm = new CParticleTrackballManipulator( m_particleSystem );
	tm->setVerticalAxisFixed( true );
    view->setCameraManipulator( tm );
        
    osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
    return gw ? gw->getGLWidget() : NULL;
}
	
osg::Camera* CViewerWidget::createCamera( int x, int y, int w, int h, const std::string& name, bool windowDecoration )
{
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();
        
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()) );
        
    camera->setClearColor( osg::Vec4(0.2, 0.2, 0.6, 1.0) );
    camera->setViewport( new osg::Viewport(0, 0, traits->width, traits->height) );
    camera->setProjectionMatrixAsPerspective(
        30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f );
    return camera.release();
}

void CViewerWidget::paintEvent( QPaintEvent* event )
{ 
	frame(); 
}

osg::Node* CViewerWidget::createScene()
{
	// create solid plane
	CSulGeomQuad* quad = new CSulGeomQuad(40, 40 );
	quad->setColor( 0.8, 0.8, 0.8, 1 );
	m_group->addChild( new CSulGeode(quad, false) );
	quad->getOrCreateStateSet()->setRenderBinDetails( 1, "RenderBin" );
	
	// create a simple grid
	CSulGeomGrid* grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-20,-20,0), 40, 40, 1, 1, 0, 0 );
	m_group->addChild( grid );
	grid->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	grid->getOrCreateStateSet()->setRenderBinDetails( 2, "RenderBin" );
	m_group->addChild( grid );

	return m_group;
}

void CViewerWidget::load()
{
	QFileDialog* dlg = new QFileDialog;
	dlg->setFileMode( QFileDialog::AnyFile );
	dlg->setNameFilter(tr("XML (*.xml)"));
	dlg->setDefaultSuffix( "xml" );
	dlg->setAcceptMode( QFileDialog::AcceptOpen );
	
	if ( dlg->exec() )
	{
		QString fileName = dlg->selectedFiles().takeFirst();
		std::string s = fileName.toUtf8().constData();
		m_particleSystem->getData()->load( s );
		m_particleSystem->updateToUI();
	}
}

void CViewerWidget::save()
{
	m_particleSystem->updateFromUI();

	QFileDialog* dlg = new QFileDialog;
	dlg->setFileMode( QFileDialog::AnyFile );
	dlg->setNameFilter(tr("XML (*.xml)"));
	dlg->setDefaultSuffix( "xml" );
	dlg->setAcceptMode( QFileDialog::AcceptSave );
	
	if ( dlg->exec() )
	{
		QString fileName = dlg->selectedFiles().takeFirst();
		std::string s = fileName.toUtf8().constData();
		m_particleSystem->getData()->save( s );
	}
}



