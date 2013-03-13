// ParticleEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulParticleSystemOsg.h>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgManipulator/Projector>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QtEvents>
#include <QtGui/QMessageBox>
#include <QtGui/QGridLayout>
#include <QtGui/qsplitter.h>

static CSulParticleSystemOsg* test = 0;
osg::Group* group = 0;

osg::Node* createScene()
{
	group = new osg::Group;

	// create solid plane
	CSulGeomQuad* quad = new CSulGeomQuad(40, 40 );
	quad->setColor( 0.8, 0.8, 0.8, 1 );
	group->addChild( new CSulGeode(quad, false) );
	quad->getOrCreateStateSet()->setRenderBinDetails( 1, "RenderBin" );
	
	// create a simple grid
	CSulGeomGrid* grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-20,-20,0), 40, 40, 1, 1, 0, 0 );
	group->addChild( grid );
	grid->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	grid->getOrCreateStateSet()->setRenderBinDetails( 2, "RenderBin" );
	group->addChild( grid );

	return group;
}

class CMyManiulator : public osgGA::TrackballManipulator
{
public:
	bool calcHitPoint(  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit )
	{
		osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
		if ( view )
		{
			osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );
			osgManipulator::PlaneProjector* planeProjector = new osgManipulator::PlaneProjector;
			planeProjector->setPlane( plane );

			osgManipulator::PointerInfo pi;
			pi.reset();
			pi.setCamera( view->getCamera() );
			pi.setMousePosition( ea.getX(), ea.getY() );
			planeProjector->project( pi, hit );
			return true;
		}

		return false;
	}

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
	{
		switch ( ea.getEventType() )
		{
			case osgGA::GUIEventAdapter::PUSH:
				osg::Vec3d hit;

				if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
				{
					if ( calcHitPoint( ea, us, hit ) )
					{
						test->updateFromUI();
						group->addChild( test->create( hit ) );
					}
				}
				return true;
		}

		return osgGA::TrackballManipulator::handle( ea, us );
	}
};

class CViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    CViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded) : QWidget()
    {
        setThreadingModel(threadingModel);

		QGridLayout* grid = new QGridLayout;
		setLayout( grid );

		QWidget* widget = addViewWidget( createCamera( 0, 0, 512, 512 ), createScene() );

		QWidget* containerPropertySheets = new QWidget;
		QVBoxLayout* layoutPropertySheets = new QVBoxLayout;
		containerPropertySheets->setLayout( layoutPropertySheets );

		test = new CSulParticleSystemOsg;
		layoutPropertySheets->addWidget( test->createPropertySheet() );

		QSplitter* splitter = new QSplitter;
		splitter->addWidget( widget );
		splitter->addWidget( containerPropertySheets );
		grid->addWidget( splitter, 0,0 );

		// we need a timer to do the osg update
		connect( &_timer, SIGNAL(timeout()), this, SLOT(update()) );
        _timer.start( 10 );
    }
    
    QWidget* addViewWidget( osg::Camera* camera, osg::Node* scene )
    {
        osgViewer::View* view = new osgViewer::View;
        view->setCamera( camera );
        addView( view );
        
        view->setSceneData( scene );
        view->addEventHandler( new osgViewer::StatsHandler );

		//osgGA::TrackballManipulator* tm = new osgGA::TrackballManipulator;
		CMyManiulator* tm = new CMyManiulator;
		tm->setVerticalAxisFixed( true );
        view->setCameraManipulator( tm );
        
        osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
        return gw ? gw->getGLWidget() : NULL;
    }
	
	osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false )
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

    virtual void paintEvent( QPaintEvent* event )
    { frame(); }

protected:    
    QTimer _timer;
};

int _tmain(int argc, _TCHAR* argv[])
{
	// Qt requires that we construct the global QApplication before creating any widgets.
    QApplication app(argc, (char**)argv);

    CViewerWidget* viewWidget = new CViewerWidget;
    viewWidget->setGeometry( 100, 100, 1024, 600 );
    viewWidget->show();

	app.exec();

	return 0;
}

