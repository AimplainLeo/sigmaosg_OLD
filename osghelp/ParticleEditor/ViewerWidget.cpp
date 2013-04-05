// ViewerWidget.cpp

#include "stdafx.h"
#include "ViewerWidget.moc"
#include "ViewerWidget.h"
#include "ParticleTrackballManipulator.h"
#include <SigmaUtil/SulParticleSystemContainerOsg.h>
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulParticleSystemContainerOsg.h>
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
	|						|				|		|					|					|	   |					|		wView2		|
	|						|				|		|					|					|	   |					|					|
	-----------------------------------------		-----------------------------------------	   -----------------------------------------

*/

CViewerWidget::CViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) : QWidget()
{
	setThreadingModel( threadingModel );

	m_dataLast = 0;

	m_group = new osg::Group;

	m_particleSystem = new CSulParticleSystemOsg;
	m_group->addChild( m_particleSystem );

	((QWidget*)this)->setWindowTitle( "Particle Editor 0.1 beta" );
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

	m_multi = new QPushButton( "Multi" );
	m_multi->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed ); 
	m_multi->setCheckable( true );
	m_multi->setStyleSheet("QPushButton {"
								"background-color: rgb(255, 0, 0);"
								"border-style:outset;"
								"border-width:2px;"
								"border-radius:10px;"
								"border-color:beige;"
								"padding: 6px;"
                            "}"
							"QPushButton:checked {"
								"background-color: rgb(0, 255, 0);"
								"border-style:outset;"
								"border-width:2px;"
								"border-radius:10px;"
								"padding: 6px;"
                            "}"
                            );	

	layoutH->addWidget( m_multi, Qt::AlignLeft );	

	layoutH->addStretch();

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
	
	// create scroll area
	m_scrollAreaPropertySheet = new QScrollArea;
	m_scrollAreaPropertySheet->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
	m_scrollAreaPropertySheet->setWidgetResizable( true );

	layout = new QVBoxLayout;
	m_wView2->setLayout( layout );
	layout->addWidget( m_scrollAreaPropertySheet );
}

QWidget* CViewerWidget::addViewWidget( osg::Camera* camera, osg::Node* scene )
{
    osgViewer::View* view = new osgViewer::View;
    view->setCamera( camera );
    addView( view );
        
    view->setSceneData( scene );
    view->addEventHandler( new osgViewer::StatsHandler );

	CParticleTrackballManipulator* tm = new CParticleTrackballManipulator( this );

	tm->setHomePosition(
			osg::Vec3(20,20,5),
			osg::Vec3(0,0,0),
			osg::Vec3(0,0,1)
			);

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

CSulParticleSystemDataOsg* CViewerWidget::createParticleSystemData( const CSulString& title, bool bEnabled )
{
	QListWidgetItem* item = new QListWidgetItem;
	item->setText( QString( title.c_str() ) );
	item->setFlags( item->flags() | Qt::ItemIsUserCheckable );
	item->setCheckState( bEnabled? Qt::Checked:Qt::Unchecked );
	m_listParticleSystem->addItem( item );

	CSulParticleSystemDataOsg* data = new CSulParticleSystemDataOsg( title );
	data->setDefaultValues();
	QVariant v = qVariantFromValue((void*)data);
	item->setData( Qt::UserRole, v );

	return data;
}

void CViewerWidget::addParticleSystem()
{
	std::string s = m_particleSystemName->text().toUtf8().constData();
	createParticleSystemData( s );
}

void CViewerWidget::removeParticleSystem()
{
	// FIXME: works, but does not delete the data associated with it.
	qDeleteAll( m_listParticleSystem->selectedItems() );
}

void CViewerWidget::clickedParticleSystem( QListWidgetItem* item )
{
	// get particle data
	QVariant v = item->data( Qt::UserRole );
	CSulParticleSystemDataOsg* data = (CSulParticleSystemDataOsg*)v.value<void *>();

	m_scrollAreaPropertySheet->takeWidget();
	m_scrollAreaPropertySheet->setWidget( data->getPropertySheet() );
}

void CViewerWidget::create( const osg::Vec3& pos )
{
	if ( !m_multi->isChecked() )
		m_particleSystem->removeAll();

	sigma::int32 count = m_listParticleSystem->count();
	for ( sigma::int32 i=0; i<count; i++ )
	{
		QListWidgetItem* item = m_listParticleSystem->item( i );

		if ( item->checkState()==Qt::Unchecked )
			continue;

		// get particle data
		QVariant v = item->data( Qt::UserRole );
		CSulParticleSystemDataOsg* data = (CSulParticleSystemDataOsg*)v.value<void *>();

		data->getPropertySheet()->updateFromUI();

		sigma::uint32 instanceCount = data->m_instanceCount;
		for ( sigma::uint32 i=0; i<instanceCount; i++ )
		{
			CSulParticleSystemContainerOsg* psContainer = new CSulParticleSystemContainerOsg( data, m_group );
			m_particleSystem->create( psContainer, pos );
		}
	}
}

bool CViewerWidget::load( const CSulString& file )
{
	CSulXmlReader reader;
	if ( !reader.load( file ) )
		return false;

	// remove all current particles
	m_listParticleSystem->clear();

	CSulXmlReader::VEC_XMLNODE vecXmlNode;
	reader.findTags( "ParticleSystemDataOsg", vecXmlNode );

	CSulXmlReader::VEC_XMLNODE::iterator i = vecXmlNode.begin();
	CSulXmlReader::VEC_XMLNODE::iterator ie = vecXmlNode.end();

	while ( i!=ie )
	{
		CSulXmlNodeTag* tagParticleSystemDataOsg = (*i);

		CSulString title = tagParticleSystemDataOsg->getAttrAsString( "Title", "" );
		bool bEnabled = tagParticleSystemDataOsg->getAttrAsBool( "Enabled", true );

		CSulParticleSystemDataOsg* data = createParticleSystemData( title, bEnabled );
		data->load( tagParticleSystemDataOsg );
		data->getPropertySheet()->updateToUI();
		
		++i;
	}

	return true;
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
		load( s );
	}
}

bool CViewerWidget::save( const CSulString& file )
{
	CSulXmlWriter	writer( "ParticleSystem" );

	sigma::int32 count = m_listParticleSystem->count();
	for ( sigma::int32 i=0; i<count; i++ )
	{
		QListWidgetItem* item = m_listParticleSystem->item( i );

		// get particle data
		QVariant v = item->data( Qt::UserRole );
		CSulParticleSystemDataOsg* data = (CSulParticleSystemDataOsg*)v.value<void *>();
		data->m_enabled = item->checkState()==Qt::Checked ? true:false;
		data->save( writer );
	}

	if ( !writer.Save( file.c_str() ) )
		return false;

	return true;
}

void CViewerWidget::save()
{
	QFileDialog* dlg = new QFileDialog;
	dlg->setFileMode( QFileDialog::AnyFile );
	dlg->setNameFilter(tr("XML (*.xml)"));
	dlg->setDefaultSuffix( "xml" );
	dlg->setAcceptMode( QFileDialog::AcceptSave );
	
	if ( dlg->exec() )
	{
		QString fileName = dlg->selectedFiles().takeFirst();
		std::string s = fileName.toUtf8().constData();
		save( s );
	}
}
