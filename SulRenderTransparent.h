// SulRenderTransparent.h

#ifndef __SULRENDERTRANSPARENT_H__
#define __SULRENDERTRANSPARENT_H__

/*
	CSulRenderTransparent vistitor( 1.0f );
	GetBodyNode()->accept( vistitor );
*/


class CSulRenderTransparent : public osg::NodeVisitor
{
public:
			CSulRenderTransparent( float fBlend );

   void		apply( osg::Geode& geode );

private:
   float	m_fBlendValue;
};

#endif // __SULRENDERTRANSPARENT_H__