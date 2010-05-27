// SceneShapeMask.h

#ifndef __SCENESHAPEMASK_H__
#define __SCENESHAPEMASK_H__

#include "SceneBase.h"

class CSceneShapeMask : public CSceneBase
{
public:
	CSceneShapeMask( const CSulString& sfile, bool bRenderMe, float lineDist );

	float getLineDist();

private:
	float m_lineDist;
};

typedef std::vector< osg::ref_ptr<CSceneShapeMask> > VEC_SHAPEMASK;

#endif // __SCENESHAPEMASK_H__