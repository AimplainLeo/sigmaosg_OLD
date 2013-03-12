// Explosion.cpp : Defines the entry point for the console application
//

#include "stdafx.h"

#include "propertySheet.h"
#include "ParticleFlyingDebris.h"
#include "particleStandard.h"

#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QtEvents>
#include <QtGui/QMessageBox>
#include <QtGui/QGridLayout>
#include <QtGui/qsplitter.h>

#include <osgQt/GraphicsWindowQt>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>


#include "ParticleDebrisSystem.h"
#include <SigmaUtil/SulGeomLines.h>
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeode.h>
#include <SigmaUtil/SulGeomBox.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulCameraManipulatorDebugger.h>
#include <SigmaUtil/SulGeomAxis.h>
#include <SigmaUtil/SulGeomPlane.h>
#include <osgViewer/Viewer>
#include <osgParticle/particle>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osg/BlendFunc>
#include <osgManipulator/Projector>
#include <osgParticle/RadialShooter>
#include <osgParticle/ModularEmitter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/FluidProgram>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ConnectedParticleSystem>
#include <osg/PositionAttitudeTransform>
#include <osgDB/FileUtils>
#include <iostream>

// convienence global variable
static osg::Group* group = 0;
static osg::Vec3 wind( 1,0,0 );
static osg::PositionAttitudeTransform* gizmo = 0;
static CSulGeomGrid* grid = 0;
static CSulGeomBox* cube = 0;
static CSulGeomPlane* plane = 0;
static CSulGeomQuad* quad = 0;

static CParticleFlyingDebris*	particleFlyingDebris = 0;
static CParticleStandard*		particleStandard = 0;

static int key = 0;

// forward declare
osg::Node* createExplosion( const osg::Vec3& pos );
osg::Node* createAnimatedInitialSmoke( const osg::Vec3& pos, int binNum );
osg::Node* createFlyingDebris( const osg::Vec3& pos, int binNum );
osg::Node* createAnimatedBurningSmoke( const osg::Vec3& pos, int binNum );
osg::Node* createFireBall( const osg::Vec3& pos, int binNum );
osg::Node* createDarkSmoke( const osg::Vec3& pos, int binNum  );

osg::Node* testEmitter( const osg::Vec3& pos )
{
	osg::Group* group = new osg::Group;

	CSulParticleEmitter* emitter = new CSulParticleEmitter;

	CSulGeomLines* lines = new CSulGeomLines;

	for (sigma::uint32 i=0; i<100; i++ )
		lines->addLine( pos, emitter->get()+pos );

	lines->createDrawable();
	
	group->addChild( new CSulGeode(lines) );

	return group;
}

CPropertySheet* g_sheetFlyingDebris = 0;

class CMyCamera : public CSulCameraManipulatorDebugger
{
public:
	CMyCamera()
	{
	}

	virtual bool handleKeyDown( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
	{
		VEC_KEY::iterator i = std::find( m_key.begin(), m_key.end(), ea.getKey() );
		if ( i==m_key.end() )
		{
			key = ea.getKey();

			// hide all propertysheet
			particleFlyingDebris->getPropertySheet()->hide();
			particleStandard->getPropertySheet()->hide();

			// show propertysheet that we are currently using
			switch (key)
			{
				case '1': 	particleFlyingDebris->getPropertySheet()->show(); break;
				case '2': 	particleStandard->getPropertySheet()->show(); break;
			}

			m_key.push_back( key );
		}

		return CSulCameraManipulatorDebugger::handleKeyDown( ea, us );
	}

	virtual bool handleKeyUp( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
	{
		VEC_KEY::iterator i = std::find( m_key.begin(), m_key.end(), ea.getKey() );
		if ( i!=m_key.end() )
			m_key.erase( i );

		return CSulCameraManipulatorDebugger::handleKeyUp( ea, us );
	}

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
	{
		switch ( ea.getEventType() )
		{
			case osgGA::GUIEventAdapter::KEYDOWN:
				return handleKeyDown( ea, us );

			case osgGA::GUIEventAdapter::KEYUP:
				return handleKeyUp( ea, us );

			case osgGA::GUIEventAdapter::PUSH:
				osg::Vec3d hit;
				if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
				{
					calcHitPoint( ea, us, hit );
					gizmo->setPosition( hit );

					switch ( key )
					{
						case '1': group->addChild( particleFlyingDebris->create( hit ) );		break;
						case '2': group->addChild( particleStandard->create( hit ) );			break;

						default:
							break;
					}

					/*
					VEC_KEY::iterator i = m_key.begin();
					VEC_KEY::iterator ie = m_key.end();
					while ( i!=ie )
					{
						switch (*i)
						{
							case '1': group->addChild( particleFlyingDebris->create( hit ) );		break;
							case '2': group->addChild( particleStandard->create( hit ) );			break;
							case '3': group->addChild( createFireBall(hit, 4000 ) );				break;
							case '4': group->addChild( createAnimatedInitialSmoke(hit, 5000)  );	break;
							case '5': group->addChild( createAnimatedBurningSmoke(hit, 6000 ) );	break;
							case '6': group->addChild( createDarkSmoke(hit, 8000 ) );				break;

							case '0' : group->addChild( testEmitter( hit ) ); break;
						}

						++i;
					}
					*/
				}
				return true;
		}

		/*
		// prevent camera manipulation if the user is pressing a key
		if ( m_key.size() )
			return true;
			*/

		return CSulCameraManipulatorDebugger::handle( ea, us );
	}

private:
	typedef std::vector<sigma::int32>	VEC_KEY;
	VEC_KEY								m_key;
};

osg::Node* createFireBall( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.5f );
	smoke->setParticleDuration( 1.0f );
	smoke->setEmitterDuration( 0.5f );
	smoke->buildEffect();

	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 4 );
	shooter->setInitialRotationalSpeedRange(
		osg::Vec3(0, 0, -1),
		osg::Vec3(0, 0, 1)
	);

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,2.5f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
	fluid->setWind( wind );

	smoke->getParticleSystem()->setDefaultAttributes( osgDB::findDataFile("images/myexp.png") );

	return smoke;
}


osg::Node* createAnimatedBurningSmoke( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(10);
    pexplosion.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    pexplosion.setAlphaRange(osgParticle::rangef(0.0f, 1.0f));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1))
	);
	pexplosion.setRadius(0.5f);
	 pexplosion.setMass(1/1.5f);
	pexplosion.setTextureTileRange(
		4, 4, 
		0, 15
	);

	// particle system
	osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes( osgDB::findDataFile("images/animated_smoke.png"), false, false);
	
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
	group->addChild(psu);

	// emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	group->addChild( emitter );
	emitter->setNumParticlesToCreateMovementCompensationRatio(1.5f);
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( 10 );
    emitter->setParticleSystem(ps);
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(10, 10);
    emitter->setCounter(counter);

	// sector
    osgParticle::SectorPlacer* placer = new osgParticle::SectorPlacer;
    placer->setCenter( pos );
    placer->setRadiusRange(0, 2.5);
	placer->setPhiRange(0, 2 * osg::PI);    // 360° angle to make a circle
    emitter->setPlacer(placer);

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    shooter->setInitialSpeedRange(0, 0);
    shooter->setInitialRotationalSpeedRange(osgParticle::rangev3(
       osg::Vec3(0, 0, -1),
       osg::Vec3(0, 0, 1)));
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setParticleSystem(ps);
	program->setFluidToAir();
	program->setWind(wind);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

osg::Node* createDarkSmoke( const osg::Vec3& pos, int binNum )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->setTextureFileName( osgDB::findDataFile("images/smoke.png")  );
	smoke->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.0f );
	smoke->setParticleDuration( 5 );
	smoke->setEmitterDuration( 2.0f );
	smoke->buildEffect();

	// make smoke dark
	osgParticle::Particle& ptemplate = smoke->getParticleSystem()->getDefaultParticleTemplate();
	ptemplate.setColorRange(osgParticle::rangev4(
                            osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f), 
                            osg::Vec4(0.2f, 0.2f, 0.2f, 0.5f)));
	

	// make smoke disapear downwards
	//ptemplate.setAlphaRange(osgParticle::rangef(1.0f, -1.0f));
	ptemplate.setAlphaRange(osgParticle::rangef(0.2f, 1.0f));

	osgParticle::ModularEmitter* me = dynamic_cast<osgParticle::ModularEmitter*>(smoke->getEmitter());

	osgParticle::RadialShooter* shooter = dynamic_cast<osgParticle::RadialShooter*>(me->getShooter());
	osgParticle::rangef r = shooter->getThetaRange();

	// I think osg has reversed the mening of theta and phi
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
	shooter->setInitialSpeedRange( 0, 4 );

	osgParticle::SectorPlacer* placer = dynamic_cast<osgParticle::SectorPlacer*>(me->getPlacer());
	placer->setRadiusRange(0.0f,3.0f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	
	fluid->setWind( wind );

	//smoke->getParticleSystem()->setDefaultAttributes( "Images/smoke.rgb" );
	//smoke->getParticleSystem()->setDefaultAttributes( "c:/smoke.png" );

	return smoke;
}

osg::Node* createLightSmoke( const osg::Vec3& pos )
{
	// dark smoke
	float scaleSmoke = 2.5f;
	float intensitySmoke = 50.0f;
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(pos, scaleSmoke, intensitySmoke);
	smoke->setParticleDuration( 1.5f );
	smoke->setEmitterDuration( 2.0f );
	smoke->getOrCreateStateSet()->setRenderBinDetails( 5000, "DepthSortedBin" );
	return smoke;
}

osg::Node* createAnimatedInitialSmoke( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	osgParticle::Particle pexplosion;
	pexplosion.setLifeTime(5);
    pexplosion.setSizeRange(osgParticle::rangef(0.75f, 3.0f));
    pexplosion.setAlphaRange(osgParticle::rangef(1.0f, 0.0f));
    pexplosion.setColorRange(osgParticle::rangev4(
        osg::Vec4(1, 1, 1, 1), 
        osg::Vec4(1, 1, 1, 1))
	);
	pexplosion.setRadius(0.5f);
	 pexplosion.setMass(1/1.5f);
	pexplosion.setTextureTileRange(
		4, 4, 
		0, 15
	);

	// particle system
	osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
	ps->setDefaultAttributes( osgDB::findDataFile("images/animated_smoke.png"), false, false);
	
    osgParticle::ParticleSystemUpdater *psu = new osgParticle::ParticleSystemUpdater;
    psu->addParticleSystem(ps);
	group->addChild(psu);

	// emitter
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
	group->addChild( emitter );
	emitter->setNumParticlesToCreateMovementCompensationRatio(1.5f);
	emitter->setEndless( false );
	emitter->setStartTime( 0 );
	emitter->setLifeTime( 5 );
    emitter->setParticleSystem(ps);
    emitter->setParticleTemplate(pexplosion);

	// counter
    osgParticle::RandomRateCounter* counter = new osgParticle::RandomRateCounter;
    counter->setRateRange(50, 50);
    emitter->setCounter(counter);

	// sector
    osgParticle::SectorPlacer *placer = new osgParticle::SectorPlacer;
    placer->setCenter( pos );
    placer->setRadiusRange(0, 2.5);
	placer->setPhiRange(0, 2 * osg::PI);    // 360° angle to make a circle
    emitter->setPlacer(placer);

	// shooter
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    shooter->setInitialSpeedRange(0, 0);
    shooter->setInitialRotationalSpeedRange(osgParticle::rangev3(
       osg::Vec3(0, 0, -1),
       osg::Vec3(0, 0, 1)));
	shooter->setThetaRange( 0.0f, osg::PI_2 );		// up/down
	shooter->setPhiRange( 0.0f, 2.0f*osg::PI );		// around
    emitter->setShooter(shooter);

	osgParticle::FluidProgram *program = new osgParticle::FluidProgram;
	program->setParticleSystem(ps);
	program->setFluidToAir();
	program->setWind(wind);
	group->addChild(program);
    
    osg::Geode *geode = new osg::Geode;
    geode->addDrawable(ps);
	group->addChild( geode );

	return group;
}

osg::Node* createImpactRing( const osg::Vec3& pos, int binNum )
{
	osg::MatrixTransform* mt = new osg::MatrixTransform;

	mt->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

	osg::Matrix m;
	m.makeTranslate( pos );
	mt->setMatrix( m );

	CSulGeomQuad* quad = new CSulGeomQuad;
	quad->setTexture( "c:/smoke.png" );

	CSulGeode* geode = new CSulGeode( quad );
	mt->addChild( geode );

	return mt;
}

/*
osg::Node* createFlyingDebris( const osg::Vec3& pos, int binNum )
{
	osg::Group* group = new osg::Group;
	group->getOrCreateStateSet()->setRenderBinDetails( binNum, "DepthSortedBin" );

	CParticleDebrisSystem* p = new CParticleDebrisSystem;
	p->init();

	osg::Matrix m;
	m.makeTranslate( pos );
	p->setMatrix( m );

	group->addChild( p );

	return group;
}
*/


osg::Node* createExplosion( const osg::Vec3& pos )
{
	osg::Group* all = new osg::Group;
	
	// problem with impact is that it needs to conform to the ground shape (have to think about this)
	//all->addChild( createImpactRing(pos, 3000 ) );

	/*
	all->addChild( createFireBall(pos, 4000 ) );
	all->addChild( createAnimatedInitialSmoke(pos, 5000) );
	all->addChild( createAnimatedBurningSmoke(pos, 6000 ) );
	*/
	

//	all->addChild( createFlyingDebris( pos, 3000 ) );
	
	

	return all;
}

osg::Node* createScene()
{
	group = new osg::Group;

	quad = new CSulGeomQuad(40, 40 );
	quad->setColor( 0.8, 0.8, 0.8, 1 );
	group->addChild( new CSulGeode(quad, false) );
	quad->getOrCreateStateSet()->setRenderBinDetails( 1, "RenderBin" );


	// create a simple grid
	grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-20,-20,0), 40, 40, 1, 1, 0, 0 );
	group->addChild( grid );
	grid->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	grid->getOrCreateStateSet()->setRenderBinDetails( 2, "RenderBin" );


	gizmo = new osg::PositionAttitudeTransform;
	gizmo->addChild( new CSulGeomAxis );
	group->addChild( gizmo );
	gizmo->getOrCreateStateSet()->setRenderBinDetails( 3, "RenderBin" );

    return group;
}

/*
int _tmain(int argc, _TCHAR* argv[])
{
	// Qt requires that we construct the global QApplication before creating any widgets.
    QApplication app(argc, (char**)argv);

    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

	CMyCamera* m = new CMyCamera;
	m->setHomePosition(
		osg::Vec3(20,20,5),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1)
		);
	viewer->setCameraManipulator( m );

    // execute main loop
//    return viewer->run();


	//QMessageBox::information(0, "Try Again", "I would really like this dialog to be in the openscenegraph view");

 // run the frame loop, interleaving Qt and the main OSG frame loop
	while(!viewer->done())
	{
		// process Qt events - this handles both events and paints the browser image
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

		viewer->frame();
	}

	return 0;
}
*/


class ViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
public:
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded) : QWidget()
    {
        setThreadingModel(threadingModel);

		particleFlyingDebris = new CParticleFlyingDebris;
		particleStandard = new CParticleStandard;

		QGridLayout* grid = new QGridLayout;
		setLayout( grid );

		QWidget* widget = addViewWidget( createCamera( 0, 0, 512, 512 ), createScene() );

		QWidget* containerPropertySheets = new QWidget;
		QVBoxLayout* layoutPropertySheets = new QVBoxLayout;
		containerPropertySheets->setLayout( layoutPropertySheets );

		// add property sheets
		layoutPropertySheets->addWidget( particleFlyingDebris->getPropertySheet() );
		layoutPropertySheets->addWidget( particleStandard->getPropertySheet() );

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

        //view->setCameraManipulator( new osgGA::TrackballManipulator );
		CMyCamera* m = new CMyCamera;
		m->setHomePosition(
			osg::Vec3(20,20,5),
			osg::Vec3(0,0,0),
			osg::Vec3(0,0,1)
			);
		view->setCameraManipulator( m );
        
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

    ViewerWidget* viewWidget = new ViewerWidget();
    viewWidget->setGeometry( 100, 100, 800, 600 );
    viewWidget->show();

	app.exec();

	return 0;
}

