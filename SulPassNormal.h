// SulPassNormal.h

#ifndef __SULPASSNORMAL_H__
#define __SULPASSNORMAL_H__

#include "SulTextureBuffer.h"
#include "SulPass.h"

class CSulPassNormal: public CSulPass
{
public:
	CSulPassNormal( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	CSulPassNormal( osg::Texture2D* pTexIn, CSulTextureBuffer* pTexOut );
	CSulPassNormal( CSulTextureBuffer* pTexIn, CSulTextureBuffer* pTexOut );
};
 


#endif // __SULPASSNORMAL_H__