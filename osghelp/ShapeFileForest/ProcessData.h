// ProcessData.h

#ifndef __PROCESSDATA_H__
#define __PROCESSDATA_H__

#include <SigmaUtil/SulGenHeightTexture.h>
#include <osg/nodecallback>
#include <osg/group>

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// this class waits a certain amount of frames (letting the .ive file time
// to load) and then starts to process the scene
//
///////////////////////////////////////////////////////////////////////////////////////////////////

class CProcessData : public osg::NodeCallback
{
public:
	CProcessData( osg::Group* pScene )
	{
		m_rScene = pScene;
		count = 0;
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    { 
		count++;

		if ( count==50 )
		{
		}

		// traverse subtree
        traverse( node, nv );
	}

private:
	int count;
	osg::ref_ptr<osg::Group> m_rScene;
};


#endif // __PROCESSDATA_H__