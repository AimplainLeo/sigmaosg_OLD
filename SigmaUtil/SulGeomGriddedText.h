// SulGeomGriddedText.h

#ifndef __SULGEOMGRIDDEDTEXT_H__
#define __SULGEOMGRIDDEDTEXT_H__

#include "SulExport.h"
#include "SulTypes.h"
#include "SulString.h"
#include "SulGeomGrid.h"
#include <osg/group>
#include <osgText/Text>

class SUL_EXPORT CSulGeomGriddedText : public osg::Group
{
public:
	enum EALIGN
	{
		ALIGN_LEFT,
		ALIGN_RIGHT
	};

public:
					CSulGeomGriddedText( sigma::uint32 cellCountX, sigma::uint32 cellCountY, sigma::uint32 cellW, sigma::uint32 cellH, sigma::uint32 viewW, sigma::uint32 viewH );

	bool			setText( const CSulString& text, sigma::uint32 line, CSulGeomGriddedText::EALIGN eAlign );

	void			setPosition( sigma::int32 x, sigma::int32 y );
	void			setColor( osg::Vec4& color );

private:
	osg::ref_ptr<osg::MatrixTransform>	m_mt;
	
	sigma::uint32	m_cellCountX;
	sigma::uint32	m_cellCountY;
	sigma::uint32	m_cellW;
	sigma::uint32	m_cellH;

	typedef std::vector< osg::ref_ptr<osgText::Text> >	VEC_TEXT;
	VEC_TEXT											m_vecText;

	osg::ref_ptr<CSulGeomGrid>							m_grid;
};

#endif // __SULGEOMGRIDDEDTEXT_H__