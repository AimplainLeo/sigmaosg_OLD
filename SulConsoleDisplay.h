// SulConsoleDisplay.h

#ifndef __SULCONSOLEDISPLAY_H__
#define __SULCONSOLEDISPLAY_H__

#include "SulExport.h"
#include "SulAnimationPath.h"
#include "SulConsoleFunctor.h"
#include "SulGeomQuad.h"
#include "SulString.h"
#include "SulTypes.h"
#include <osg/Referenced>
#include <osgText/Text>
#include <osgViewer/Viewer>

class SUL_EXPORT CSulConsoleDisplay : public osg::Referenced
{
private:
	typedef std::vector< osg::ref_ptr<CSulConsoleFunctorBase> >		VECTOR_CONSOLEFUNCTOR;
	typedef std::vector< osg::ref_ptr<osgText::Text> >				VECTOR_TEXT;
	typedef std::vector< CSulString >								VECTOR_STRING;

public:
											CSulConsoleDisplay( osgViewer::Viewer* pViewer );

	void									Init();

	void									SetBgColor( float r, float g, float b, float a );
	const osg::Vec4&						GetBgColor();
	void									GetBgColor( float& r, float& g, float& b, float& a );

	void									SetActivateKey( sigma::int32 iKey );
	int										GetActivateKey();

	void									Show( bool bShow );
	bool									IsVisible();
	
	// creates visible text objects in view
	void									CreateTextObjects( sigma::uint32 iCount );

	void									ExecuteCommand( const CSulString& sCmd );
	void									WantCommand( CSulConsoleFunctorBase* pFunctor );

	void									DisplayText( const CSulString& sText );

	void									Input( sigma::int32 key, sigma::int32 iMod );

	void									SetCursor( const CSulString& s );

	osg::Node*								GetNode();

private:
	void									SetActive( bool bActive );
	void									BlindCheck();
	void									UpdateTextObjects();
	void									UpdateText();

	void									PageUp();
	void									PageDown();
	void									MarkerUp();
	void									MarkerDown();
	void									ScrollUp();
	void									ScrollDown();

private:
	VECTOR_CONSOLEFUNCTOR					m_vecConsoleFunctor;
	osg::ref_ptr<osgViewer::Viewer>			m_rViewer;
	osg::ref_ptr<CSulGeomQuad>				m_rQuad;
	osg::ref_ptr<osg::Geode>				m_rGeode;
	osg::ref_ptr<osg::Node>					m_rNode;
	osg::ref_ptr<osg::AnimationPath>		m_rPath;
	osg::ref_ptr<CSulAnimationPathCallback>	m_rAniPathCallback;
	bool									m_bShow;
	bool									m_bActive;

	osg::ref_ptr<osgText::Text>				m_rText;
	CSulString								m_sText;
	CSulString								m_sCursor;
	VECTOR_TEXT								m_vecTextObject;
	VECTOR_STRING							m_vecTextBuffer;
	VECTOR_STRING							m_vecTextHistory;

	sigma::uint32							m_iSpacingV;
	sigma::uint32							m_iTextObjs;
	sigma::int32							m_h;
	sigma::int32							m_w;
	sigma::int32							m_iActivateKey;
	float									m_fCharSize;

	sigma::int32							m_iMarkerPos;
	sigma::int32							m_iScrollPos;
};

#endif // __SULCONSOLEDISPLAY_H__