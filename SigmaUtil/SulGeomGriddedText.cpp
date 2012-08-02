// SulGeomGriddedText.cpp

#include "stdafx.h"
#include "SulGeomGriddedText.h"

CSulGeomGriddedText::CSulGeomGriddedText( sigma::uint32 cellCountX, sigma::uint32 cellCountY, sigma::uint32 cellW, sigma::uint32 cellH, sigma::uint32 viewW, sigma::uint32 viewH ) :
osg::Group()
{
	m_cellCountX = cellCountX;
	m_cellCountY = cellCountY;
	m_cellW = cellW;
	m_cellH = cellH;
	
	////////////////////////////////////////////////////////////////////////////////
	// create grid
	////////////////////////////////////////////////////////////////////////////////

	m_grid = new CSulGeomGrid;
	m_grid->Create( osg::Vec3(0,0,0), cellCountX, cellCountY, cellW, cellH, 0, 0 );

	////////////////////////////////////////////////////////////////////////////////
	// create transformation into screen space
	////////////////////////////////////////////////////////////////////////////////

	m_mt = new osg::MatrixTransform;
	m_mt->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_mt->setMatrix( osg::Matrix::identity() );
	m_mt->addChild( m_grid );
	
	osg::Projection* proj = new osg::Projection;
	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, viewW, 0, viewH );
	proj->setMatrix( mOrtho );
	proj->addChild( m_mt );
	addChild( proj );

	////////////////////////////////////////////////////////////////////////////////
	// create text for each cell (wish osgText had better spacing control between
	// characters
	////////////////////////////////////////////////////////////////////////////////

	osg::Geode* pGeodeText = new osg::Geode();
	m_mt->addChild( pGeodeText );

	for ( sigma::uint32 y=0; y<cellCountY; y++ )
		for ( sigma::uint32 x=0; x<cellCountX; x++ )
		{
			osgText::Text* pText = new  osgText::Text;
			pText->setFont( "fonts/DroidSansMono.ttf" );

			pText->setAlignment( osgText::TextBase::CENTER_CENTER );

			pText->setCharacterSize( cellH );
			pText->setAxisAlignment( osgText::Text::SCREEN );
			pText->setCharacterSizeMode( osgText::Text::SCREEN_COORDS );
			pText->setPosition( osg::Vec3( (cellW/2)+x*cellW, (cellH/2)+y*cellH, 0 ) );
			pText->setColor( osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
			pText->setText( "W" );
			pGeodeText->addDrawable( pText );

			m_vecText.push_back( pText );
		}

	////////////////////////////////////////////////////////////////////////////////
	// turn off lighting, has always been a pain to decide if I should do it here
	// or let the application that uses this object handle it.
	////////////////////////////////////////////////////////////////////////////////

	getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
}

bool CSulGeomGriddedText::setText( const CSulString& text, sigma::uint32 line, CSulGeomGriddedText::EALIGN eAlign )
{
	///////////////////////////
	// sanity checks
	///////////////////////////

	if ( text.size()>m_cellCountX )
		return false;

	if ( line>(m_cellCountY-1) )
		return false;

	///////////////////////////
	// calc character start
	///////////////////////////

	sigma::uint32 index = 0;
	index+= line*m_cellCountX;

	///////////////////////////
	// erase current line
	///////////////////////////

	for ( sigma::uint32 ii=0; ii<m_cellCountX; ii++ )
	{
		m_vecText[index+ii]->setText( "" );
	}

	///////////////////////////
	// adjust character start
	///////////////////////////

	if ( eAlign==ALIGN_RIGHT )
	{
		index+= m_cellCountX-text.size();
	}

	///////////////////////////
	// extract each character
	///////////////////////////

	for ( sigma::uint32 x=0; x<text.size(); x++ )
	{
		m_vecText[index+x]->setText( CSulString( (sigma::uint8)text[x]) );
	}

	return true;
}

void CSulGeomGriddedText::setPosition( sigma::int32 x, sigma::int32 y )
{
	osg::Matrix m;

	m.setTrans( x, y, 0 );

	m_mt->setMatrix( m );
}

void CSulGeomGriddedText::setColor( osg::Vec4& color )
{
	VEC_TEXT::iterator i = m_vecText.begin();
	VEC_TEXT::iterator ie = m_vecText.end();

	while ( i!=ie )
	{
		(*i)->setColor( color );
		++i;
	}

	m_grid->setColor( color );
}

