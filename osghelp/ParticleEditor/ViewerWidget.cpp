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
#include <QtGui/qlistview>
#include <QtGui/qlistwidget>
#include <QtGui/qlayout>
#include <QtGui/qlineedit>
#include <QtGui/QListWidgetItem>

/*
																	layoutBase
	-----------------------------------------		-----------------------------------------      -----------------------------------------
	|			buttons/toolbar				|		|			  widgetToolbar				|	   |			widgetToolbar				|
	-----------------------------------------		-----------------------------------------	   -----------------------------------------
	|						|				|		|					|	 				|	   |					|					|
	|						|	particle	|		|					|					|	   |					|					|
	|						|	systems		|		|					|					|	   |					|		wView1		|
	|						|				|		|			widgetHSplitter				|	   |					|					|
	|		test view		|---------------|		|					|					|	   |	  wView0		|--widgetVSplitter--|
	|						|				|		|					|					|	   |					|					|
	|						|	properties	|		|					|					|	   |					|					|
	|						|				|		|					|					|	   |					|		wView2			|
	|						|				|		|					|					|	   |					|					|
	-----------------------------------------		-----------------------------------------	   -----------------------------------------

*/

CViewerWidget::CViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
	setThreadingModel( threadingModel );

	m_psContainer = 0;

	m_group = new osg::Group;

	m_particleSystem = new CSulParticleSystemOsg;
	m_group->addChild( m_particleSystem );

	createLayout();
	createViews();

	// we need a timer to do the osg update
	connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
    _timer.start( 10 );
}

void CViewerWidget::createLayout()
{
	m_wView0 = new QWidget;
	m_wView1 = new QWidget;
	m_wView2 = new QWidget;
	m_widgetToolbar = new QWidget;

	/*
	m_wView0->setStyleSheet("background-color:blue;");
	m_wView1->setStyleSheet("background-color:red;");
	m_wView2->setStyleSheet("background-color:yellow;");
	m_widgetToolbar->setStyleSheet("background-color:green;");
	*/

	QVBoxLayout* layoutBase = new QVBoxLayout;
	setLayout( layoutBase );
	
	layoutBase->addWidget( m_widgetToolbar );

	QSplitter* widgetHSplitter = new QSplitter;
	layoutBase->addWidget( widgetHSplitter );

	QSplitter* widgetVSplitter = new QSplitter;
	widgetVSplitter->setOrientation( Qt::Vertical );

	widgetHSplitter->addWidget( m_wView0 );
	widgetHSplitter->addWidget( widgetVSplitter );

	widgetVSplitter->addWidget( m_wView1 );
	widgetVSplitter->addWidget( m_wView2 );
}

void CViewerWidget::createViews()
{
	/////////////////////////////////////////////////
	// toolbar
	/////////////////////////////////////////////////

	QHBoxLayout* layoutH = new QHBoxLayout;
	m_widgetToolbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed ); 
	m_widgetToolbar->setLayout( layoutH );

	QPushButton* load = new QPushButton( "Load" );
	load->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed ); 
	layoutH->addWidget( load, Qt::AlignLeft );	
	connect( load, SIGNAL(clicked()), this, SLOT(load()) );

	QPushButton* save = new QPushButton( "Save" );
	save->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed ); 
	layoutH->addWidget( save, Qt::AlignLeft );	
	connect( save, SIGNAL(clicked()), this, SLOT(save()) );

	/////////////////////////////////////////////////
	// 3D view
	/////////////////////////////////////////////////

	QVBoxLayout* layout = new QVBoxLayout;
	m_wView0->setLayout( layout );
	m_wView0->setMinimumWidth( 1024 );
	layout->addWidget( addViewWidget( createCamera( 0, 0, 512, 512 ), createScene() ) );

	/////////////////////////////////////////////////
	// particle system listbox
	/////////////////////////////////////////////////

	layout = new QVBoxLayout;
	m_wView1->setLayout( layout );

	QWidget* w = new QWidget;
	QHBoxLayout* layoutListBoxEdit = new QHBoxLayout;
	w->setLayout( layoutListBoxEdit );
	layout->addWidget( w );
	QPushButton* buttonAdd = new QPushButton("Add");
	m_particleSystemName = new QLineEdit;
	QPushButton* buttonRemove = new QPushButton("Remove");
	layoutListBoxEdit->addWidget( m_particleSystemName );
	layoutListBoxEdit->addWidget( buttonAdd );
	layoutListBoxEdit->addWidget( buttonRemove );

	m_listParticleSystem = new QListWidget;
	layout->addWidget( m_listParticleSystem );

	connect( buttonAdd, SIGNAL(clicked()), this, SLOT(addParticleSystem()) );
	connect( buttonRemove, SIGNAL(clicked()), this, SLOT(removeParticleSystem()) );

	connect( m_listParticleSystem, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(clickedParticleSystem(QListWidgetItem*)) );
	
	/////////////////////////////////////////////////
	// property sheet
	/////////////////////////////////////////////////
	
	// create property sheet
	QWidget* widgetPropertySheet = m_particleSystem->createPropertySheet();

	// create scroll area
	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	scrollArea->setWidget( widgetPropertySheet );
	scrollArea->setWidgetResizable( true );

	layout = new QVBoxLayout;
	m_wView2->setLayout( layout );
	layout->addWidget( scrollArea );
}

QWidget* CViewerWidget::addViewWidget( osg::Camera* camera, osg::Node* scene )
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera( camera );
    addView( view );
        
    view->setSceneData( scene );
    view->addEventHandler( new osgViewer::StatsHandler );

	//CParticleTrackballManipulator* tm = new CParticleTrackballManipulator( m_particleSystem );
	CParticleTrackballManipulator* tm = new CParticleTrackballManipulator( this );
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

void CViewerWidget::addParticleSystem()
{
	QListWidgetItem* item = new QListWidgetItem;
	item->setText( m_particleSystemName->text() );
	item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
	item->setCheckState( Qt::Checked );
	m_listParticleSystem->addItem( item );

	CSulParticleSystemDataOsg* data = new CSulParticleSystemDataOsg;
	data->setDefaultValues();

	QVariant v = qVariantFromValue((void*)data);

	item->setData( Qt::UserRole, v );
}

void CViewerWidget::removeParticleSystem()
{
	// FIXME: works, but does not delete the data associated with it.
	qDeleteAll( m_listParticleSystem->selectedItems() );
}

void CViewerWidget::clickedParticleSystem( QListWidgetItem* item )
{
	//m_particleSystem->updateFromUI();

	// get particle data
	QVariant v = item->data( Qt::UserRole );
	CSulParticleSystemDataOsg* data = (CSulParticleSystemDataOsg*)v.value<void *>();

	m_particleSystem->setData( data );
	m_particleSystem->updateToUI();
}

void CViewerWidget::place( const osg::Vec3& pos )
{
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator i = m_vecParticleSystemContainer.begin();
	VEC_PARTICLESYSTEMCONTAINEROSG::iterator ie = m_vecParticleSystemContainer.end();

	while ( i!=ie )
	{
		m_particleSystem->placeParticleSystem( pos, (*i) );
		++i;
	}
}

void CViewerWidget::create()
{
	m_vecParticleSystemContainer.clear();

	// we need to create one for each in the list
	m_particleSystem->updateFromUI();
	sigma::int32 count = m_listParticleSystem->count();
	for ( sigma::int32 i=0; i<count; i++ )
	{
		QListWidgetItem* item = m_listParticleSystem->item( i );

		// get particle data
		QVariant v = item->data( Qt::UserRole );
		CSulParticleSystemDataOsg* data = (CSulParticleSystemDataOsg*)v.value<void *>();

		// create particle container
		m_particleSystem->setData( data );
		CSulParticleSystemContainerOsg* psContainer = m_particleSystem->createParticleSystem();
		m_vecParticleSystemContainer.push_back( psContainer );
	}
}

