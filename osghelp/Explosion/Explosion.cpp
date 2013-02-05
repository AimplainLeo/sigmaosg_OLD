// Explosion.cpp : Defines the entry point for the console application
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomGrid.h>
#include <SigmaUtil/SulGeode.h>
#include <SigmaUtil/SulScreenAlignedQuad.h>
#include <SigmaUtil/SulManipulatorCamera.h>
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

// convienence global variable
static osg::Group* group = 0;

osg::Node* createExplosion( const osg::Vec3& pos );

class CInputHandler : public osgGA::GUIEventHandler 
{
public:
	CInputHandler()
	{
		osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );
		m_rPlaneProj = new osgManipulator::PlaneProjector;
		m_rPlaneProj->setPlane( plane );
	}

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( !pViewer )
        {
            return false;
        }

		if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
        {
            osg::Camera* cam = pViewer->getCamera();

			osg::Vec3d v;
			osgManipulator::PointerInfo pi;
		
			pi.reset();
			pi.setCamera( cam );
			pi.setMousePosition( ea.getX(), ea.getY() );
			m_rPlaneProj->project( pi, v );
			
			group->addChild( createExplosion( v ) );

            return true; // return true, event handled
        }

        return false;
    }

private:
	osg::ref_ptr<osgManipulator::PlaneProjector> m_rPlaneProj;
};


osg::Node* createFireBall( const osg::Vec3& pos )
{
	// fire
	float scale = 1.0f;
	float intensity = 20.0f;
	osgParticle::SmokeEffect* fire =  new osgParticle::SmokeEffect(pos, scale, intensity);
	fire->setTextureFileName( "c:/myexp.png" );
	fire->setParticleDuration( 0.3f );
	fire->setEmitterDuration( .1f );
	return fire;
}

osg::Node* createFireBall2( const osg::Vec3& pos )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( 5000, "DepthSortedBin" );
	//smoke->setTextureFileName( "c:/myexp.png" );
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

	smoke->getParticleSystem()->setDefaultAttributes( "c:/myexp.png" );

	return smoke;
}


osg::Node* createDarkSmoke( const osg::Vec3& pos )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( 10000, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.5f );
	smoke->setParticleDuration( 20.5f );
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
	placer->setRadiusRange(0.0f,2.0f);

	osgParticle::FluidProgram* fluid = dynamic_cast<osgParticle::FluidProgram*>(smoke->getProgram());
	fluid->setFluidDensity(1.0f);	

	//smoke->getParticleSystem()->setDefaultAttributes( "Images/smoke.rgb" );

	return smoke;
}

osg::Node* createDarkSmoke2( const osg::Vec3& pos )
{
	osgParticle::SmokeEffect* smoke =  new osgParticle::SmokeEffect(false);
	smoke->getOrCreateStateSet()->setRenderBinDetails( 4000, "DepthSortedBin" );
	smoke->setPosition( pos );
	smoke->setIntensity( 50.0f );
	smoke->setScale( 1.5f );
	smoke->setParticleDuration( 20.5f );
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

	//smoke->getParticleSystem()->setDefaultAttributes( "Images/smoke.rgb" );

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

osg::Node* createExplosion( const osg::Vec3& pos )
{
	osg::Group* all = new osg::Group;
	
	/*
	// additive blending
	osg::StateSet* ss = all->getOrCreateStateSet();
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	osg::BlendFunc* trans = new osg::BlendFunc();
	trans->setFunction( osg::BlendFunc::ONE ,osg::BlendFunc::ONE );
	ss->setAttributeAndModes( trans );

	ss->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	*/
	

	//all->addChild( createFireBall(pos) );
	all->addChild( createFireBall2(pos) );
	//all->addChild( createDarkSmoke2(pos) );
	//all->addChild( createLightSmoke(pos) );



	return all;
}

osg::Node* createScene()
{
	group = new osg::Group;

	// create a simple grid
	CSulGeomGrid* grid = new CSulGeomGrid;
	grid->Create( osg::Vec3(-2,-2,0), 40, 40, 1, 1, 5, 5 );
	group->addChild( grid );

	/*
	// transparent blending
	osg::StateSet* ss = group->getOrCreateStateSet();
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );
	osg::BlendFunc *trans = new osg::BlendFunc();
	trans->setFunction(osg::BlendFunc::SRC_ALPHA ,osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	ss->setAttributeAndModes( trans );
	*/

	//osgParticle::SmokeTrailEffect* smoke =  new osgParticle::SmokeTrailEffect(position, scale, intensity);

	/*
	CSulScreenAlignedQuad* p = new CSulScreenAlignedQuad( 512, 512, "c:/explosion.png" );  
	//CSulScreenAlignedQuad* p = new CSulScreenAlignedQuad( 512, 512, "c:/myexp.png" );  
	group->addChild( p );

	osg::StateSet* ss = p->getOrCreateStateSet();
	ss->setMode( GL_BLEND, osg::StateAttribute::ON );

	osg::BlendFunc *trans = new osg::BlendFunc();
	trans->setFunction(osg::BlendFunc::SRC_ALPHA ,osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
	ss->setAttributeAndModes( trans );
	*/


    return group;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( createScene() );

    // add the handler to the viewer
    viewer->addEventHandler( new CInputHandler );

	CSulManipulatorCamera* m = new CSulManipulatorCamera;
	m->setHomePosition(
		osg::Vec3(20,20,5),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1)
		);
	viewer->setCameraManipulator( m );

    // execute main loop
    return viewer->run();
}
