// SulClipTriangle.h

#ifndef __SULCLIPTRIANGLE_H__
#define __SULCLIPTRIANGLE_H__

#include "SulDataTri.h"
#include "SulTypes.h"
#include <osg/plane>

class CSulClipTriangle 
{
public:
						CSulClipTriangle();

						CSulClipTriangle( const CSulDataTri& tri, const osg::Plane& plane );

						CSulClipTriangle( const sigma::VEC_TRI& vecTri, const osg::Plane& plane );

	sigma::uint32		getCount();
	const CSulDataTri*	getTriangle( sigma::uint32 index );

	const sigma::VEC_TRI&		getTriangleList() const;

private:
	void				processClipTriPlane( const CSulDataTri& tri, const osg::Plane& plane );
	void				processSingleTri( const CSulDataTri& tri );
	void				process();

	int					testVert( const osg::Vec3& v );

private:
	osg::Plane			m_plane;
	sigma::VEC_TRI		m_vecTriIn;	
	sigma::VEC_TRI		m_vecTriOut;
};

#endif // __SULCLIPTRIANGLE_H__