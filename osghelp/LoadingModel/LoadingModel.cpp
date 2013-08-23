// LoadingModel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "myloader.h"
#include <SigmaUtil/SulNodeFinder.h>
#include <SigmaUtil/SulGeodeSphere.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>

#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/RigGeometry>
#include <osgAnimation/RigTransformHardware>
#include <osgAnimation/UpdateBone>
#include <osgAnimation/StackedQuaternionElement>

osg::Node* createSceneOrg()
{
	// load model
	//osgDB::setDataFilePathList("c:/deleteme/obj");

	//osg::Node* pLoadedModel = osgDB::readNodeFile( "c:/deleteme/fbx_ascii/Broadleaf_RT.fbx" );
	//osg::Node* pLoadedModel = osgDB::readNodeFile( "C:/deleteme/obj/Broadleaf_RT.obj" );
	//osg::Node* pLoadedModel = osgDB::readNodeFile( "c:/deleteme/BroadLeaf_HighDetail.obj" );
	//osg::Node* pLoadedModel = osgDB::readNodeFile( "torusknot.obj" );

	
	//osg::Node* pLoadedModel = osgDB::readNodeFile( osgDB::findDataFile( "test.dae" ) );

	
	//osgDB::writeNodeFile( *pLoadedModel , "test.osg" );

	// THE SKELETON!?!?!?! there are two!!!!

	osg::Group* group = new osg::Group;

	// BVH animation data
	osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "test.bvh" ) );
	//osgAnimation::AnimationManagerBase* animationManager = dynamic_cast<osgAnimation::AnimationManagerBase*>(bvh->getUpdateCallback());
	//group->addChild( bvh );
	
	// MODEL
	osg::Node* model = osgDB::readNodeFile( osgDB::findDataFile( "test.dae" ) );
	group->addChild( model );
	
	//model->setUpdateCallback( animationManager );
	
	/*
	// find skeleton 
	osgAnimation::Skeleton* skeleton = CSulNodeFinder::findNodeByType<osgAnimation::Skeleton>(bvh);
	skeleton->addChild( model );
	*/

	//animationManager->link( model );

	/*
	osg::ref_ptr<osgAnimation::BasicAnimationManager> manager = dynamic_cast<osgAnimation::BasicAnimationManager*>(animationManager);
	osgAnimation::Animation* anim = animationManager->getAnimationList()[0];
	anim->setPlayMode( osgAnimation::Animation::LOOP );
	manager->playAnimation( anim );
	*/
	
	/*
	// animation controller
    osg::ref_ptr<osgAnimation::BasicAnimationManager> manager = new osgAnimation::BasicAnimationManager;
    group->setUpdateCallback( manager );

	
	// animation
	osgAnimation::Animation* anim = animationManager->getAnimationList()[0];

	manager->link( model );

    manager->registerAnimation(anim);
    manager->buildTargetReference();
	manager->playAnimation(anim);
	*/

	return group;
}

class CAddDummyObject : public osg::NodeVisitor
{
public:
	CAddDummyObject() : 
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
		{}

	void apply( osg::Node& node )
    {
		osgAnimation::Bone* bone = dynamic_cast<osgAnimation::Bone*>(&node);
		if ( bone )
		{
			osg::Sphere* shape = new osg::Sphere( osg::Vec3(0, 0, 0), 1.0f );
			osg::ShapeDrawable* d = new osg::ShapeDrawable( shape );
			osg::Geode* geodeSphere = new osg::Geode;
			geodeSphere->addDrawable(d);
			bone->addChild( geodeSphere );

			osgAnimation::UpdateBone* ub = dynamic_cast<osgAnimation::UpdateBone*>(bone->getUpdateCallback());
            osgAnimation::StackedTransform& stack = ub->getStackedTransforms();
            stack.push_back( new osgAnimation::StackedQuaternionElement("quaternion", osg::Quat()) );
		}
		traverse(node);
	}

};

osg::Node* createScene()
{
	/*
	osg::Node* test	= osgDB::readNodeFile( osgDB::findDataFile( "test.bvh" ) );
	test->accept( *new CAddDummyObject );
	return test;

	osg::Node* node =  osgDB::readNodeFile( osgDB::findDataFile( "test.osg" ) );
	node->accept( *new CAddDummyObject );
	return node;
	*/

	/*
	osgDB::ReaderWriter::Options* o = new osgDB::ReaderWriter::Options;
	o->setOptionString( "solids" );

	
	//osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "01_01.bvh2" ), o );
	//osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "test.bvh2" ), o );
	//osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "homemade.bvh2" ), o );

	osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "homemade.bvh" ), o );
	//osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "test.bvh" ), o );
	//osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "testunity.bvh" ), o );
	//osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "01_01.bvh" ), o );

	osgDB::writeNodeFile( *bvh , "test_homemade.osg" );

	return bvh;

	bvh->accept( *new CAddDummyObject );
	return bvh;

	osg::ref_ptr<osgAnimation::BasicAnimationManager> manager = dynamic_cast<osgAnimation::BasicAnimationManager*>(bvh->getUpdateCallback());

	osgAnimation::Animation* anim = manager->getAnimationList()[0];
	anim->setPlayMode( osgAnimation::Animation::LOOP );
	manager->playAnimation( anim );

	

	return bvh;
	*/

	osg::Node*								model		= osgDB::readNodeFile( osgDB::findDataFile( "test.dae" ) );
	osg::Node*								bvh			= osgDB::readNodeFile( osgDB::findDataFile( "test.bvh" ) );
	osgAnimation::Skeleton*					skeleton	= CSulNodeFinder::findNodeByType<osgAnimation::Skeleton>(model);
	//osgAnimation::BasicAnimationManager*	bam			= CSulNodeFinder::findNodeByType<osgAnimation::BasicAnimationManager>(bvh);



	osgAnimation::AnimationManagerBase* animationManager = dynamic_cast<osgAnimation::AnimationManagerBase*>(bvh->getUpdateCallback());
	osgAnimation::Animation* anim = animationManager->getAnimationList()[0];
	osgAnimation::BasicAnimationManager* bam = new osgAnimation::BasicAnimationManager;
	bam->registerAnimation( anim );
    bam->buildTargetReference();

	anim->setPlayMode( osgAnimation::Animation::LOOP );
	bam->playAnimation( anim );

	osg::Group* group = new osg::Group;
	
	skeleton->accept( *new CAddDummyObject );
		
	group->setUpdateCallback( bam );
	group->addChild( skeleton );

	//osgDB::writeNodeFile( *group , "test.osg" );

	return group;
}


osg::Node* createScene2()
{
	/*
	osg::Node* test = osgDB::readNodeFile( osgDB::findDataFile( "nathan.osg" ) );
	return test;
	*/

	osg::Group* group = new osg::Group;

	// convenience variables
	osg::Node* bvh = osgDB::readNodeFile( osgDB::findDataFile( "test.bvh" ) );
	osg::Node* model = osgDB::readNodeFile( osgDB::findDataFile( "test.dae" ) );
	osgAnimation::BasicAnimationManager* manager = new osgAnimation::BasicAnimationManager;
	manager->setAutomaticLink( true );

	model->accept( *new CAddDummyObject );

	group->setUpdateCallback( manager );
	group->addChild( model );

	// get animation from bvh
	osgAnimation::AnimationManagerBase* animationManager = dynamic_cast<osgAnimation::AnimationManagerBase*>(bvh->getUpdateCallback());
	osgAnimation::Animation* anim = animationManager->getAnimationList()[0];
	manager->registerAnimation( anim );
    manager->buildTargetReference();
	//manager->link( model );
    //manager->playAnimation( anim );

	//osgAnimation::Skeleton* skeleton = CSulNodeFinder::findNodeByType<osgAnimation::Skeleton>(model);

	//osgDB::writeNodeFile( *group , "mymodel.osg" );

	manager->playAnimation( anim );

	return group;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// construct the viewer
	osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

	// make the viewer create a 512x512 window and position it at 32, 32
	rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	rViewer->setSceneData( createScene() );

	// execute main loop
	return rViewer->run();
}
