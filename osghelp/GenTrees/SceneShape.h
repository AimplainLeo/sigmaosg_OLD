// SceneShape.h

#ifndef __SCENESHAPE_H__
#define __SCENESHAPE_H__

#include "SceneBase.h"

class CSceneShape : public CSceneBase
{
public:
	CSceneShape( const CSulString& sfile, bool bRenderMe );

	CSceneShape( float w, float h, bool bRenderMe );
};

#endif // __SCENESHAPE_H__