# Introduction #

Using osg::AnimationPath makes it easy to have objects move from different points in the scene over time.

# Details #

Use osg::AnimationPath::ControlPoint to create points that you want your object to move to when osg::AnimationPath is active.

# Code #

```
#include <osgViewer/Viewer>
#include <osg/animationpath>
#include <osg/matrixtransform>
#include <osg/ShapeDrawable>

osg::Node* CreateScene()
{
    osg::Geode* pGeode = new osg::Geode();
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );

    osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
    pMatTrans->addChild( pGeode );
    
	osg::ref_ptr<osg::AnimationPath> rPath = new osg::AnimationPath;
	rPath->setLoopMode( osg::AnimationPath::SWING );
	osg::AnimationPath::ControlPoint c0(osg::Vec3(-1,0,0));
	osg::AnimationPath::ControlPoint c1(osg::Vec3( 1,0,0));
	rPath->insert( 0.0f, c0 );
	rPath->insert( 2.0f, c1 );
	
	osg::ref_ptr<osg::AnimationPathCallback> rAniCallback = new osg::AnimationPathCallback( rPath.get() );
	pMatTrans->setUpdateCallback( rAniCallback.get() );

    return pMatTrans;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}
```

# Libraries #

osg.lib osgViewer.lib

# Previews #



wiki:video: cannot find YouTube video id within parameter "url".

