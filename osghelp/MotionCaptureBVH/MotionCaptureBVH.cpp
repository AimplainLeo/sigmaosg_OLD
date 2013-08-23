// MotionCaptureBVH.cpp : Defines the entry point for the console application.
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
			/*
			// for testing
			osg::Sphere* shape = new osg::Sphere( osg::Vec3(0, 0, 0), 1.0f );
			osg::ShapeDrawable* d = new osg::ShapeDrawable( shape );
			osg::Geode* geodeSphere = new osg::Geode;
			geodeSphere->addDrawable(d);
			bone->addChild( geodeSphere );
			*/

			osgAnimation::UpdateBone* ub = dynamic_cast<osgAnimation::UpdateBone*>(bone->getUpdateCallback());
            osgAnimation::StackedTransform& stack = ub->getStackedTransforms();
            stack.push_back( new osgAnimation::StackedQuaternionElement("quaternion", osg::Quat()) );
		}
		traverse(node);
	}

};

osg::Node* createScene()
{
	osg::Node*								model		= osgDB::readNodeFile( osgDB::findDataFile( "test.dae" ) );
	osg::Node*								bvh			= osgDB::readNodeFile( osgDB::findDataFile( "test.bvh" ) );
	//osg::Node*								bvh			= osgDB::readNodeFile( osgDB::findDataFile( "data/cmuconvert-daz-01-09/01/01_01.bvh2" ) );
	osgAnimation::Skeleton*					skeleton	= CSulNodeFinder::findNodeByType<osgAnimation::Skeleton>(model);

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
