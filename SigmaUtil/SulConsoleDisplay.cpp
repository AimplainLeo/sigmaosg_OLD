// SulConsoleDisplay.cpp

#include "stdafx.h"
#include "SulConsoleDisplay.h"
#include "SulTransScreenAlign.h"

#include "SulConsoleInputHandler.h"

// used to make the cursor blink every second
class CCursorUpdate : public osg::NodeCallback
{
public:
	CCursorUpdate( CSulConsoleDisplay* p )
	{
		m_rDisplay = p;
	}

	virtual void operator() ( osg::Node* node, osg::NodeVisitor* nv )
    {
		traverse(node,nv);

        int i = ((int)osg::Timer::instance()->time_s()%2);
		m_rDisplay->SetCursor( i?"_":" " );
	}

	osg::ref_ptr<CSulConsoleDisplay>	m_rDisplay;
};

CSulConsoleDisplay::CSulConsoleDisplay( osgViewer::Viewer* pViewer )
{
	m_rViewer		= pViewer;
	m_bShow			= false;
	m_bActive		= false;
	m_iSpacingV		= 16;
	m_iTextObjs		= 16;
	m_sCursor		= "_";
	m_iActivateKey	= -67;
	m_iMarkerPos	= -1;
	m_iScrollPos	= 0;
	m_fCharSize		= 20.0f;
}

void CSulConsoleDisplay::Init()
{
	// get size of window
	osgViewer::Viewer::Windows windows;
	m_rViewer->getWindows(windows);
	osgViewer::GraphicsWindow* window = 0;
	window = windows.front();
	m_w = window->getTraits()->width;
	m_h = window->getTraits()->height;

	// create a quad for our display
	m_rQuad = new CSulGeomQuad( osg::Vec3(m_w/2,m_h+m_h/4,0), m_w, m_h/2 );
	m_rQuad->setColor( 0.0f, 0.0f ,0.0f, 0.5f );
	m_rQuad->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	m_rQuad->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	m_rQuad->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

    // create geometry node that will contain all our drawables
    m_rGeode = new osg::Geode;

	osg::ref_ptr<CSulTransScreenAlign> rAlign = new CSulTransScreenAlign( m_w, m_h );
	rAlign->AddChild( m_rGeode );
	rAlign->AddChild( m_rQuad );

	// save node so it can be added to a scene
	m_rNode = rAlign->GetProjection();

	// create an animation path that allows us to scroll the display up and down
	osg::AnimationPath::ControlPoint c0(osg::Vec3(0,0,0));
	osg::AnimationPath::ControlPoint c1(osg::Vec3(0,-m_h/2,0));
	m_rPath = new osg::AnimationPath;
	m_rPath->insert( 0.0, c0 );
	m_rPath->insert( 0.5, c1 );
	m_rAniPathCallback = new CSulAnimationPathCallback;
	m_rPath->setLoopMode( osg::AnimationPath::NO_LOOPING );
	m_rAniPathCallback->SetPath( m_rPath.get() );
	rAlign->GetMatrixTransform()->addUpdateCallback( m_rAniPathCallback.get() );
	
	// create a text we will use for input
	m_rText = new osgText::Text;
	m_rText->setCharacterSize( m_fCharSize );
	m_rText->setFont( "Tempest.ttf" );
	m_rText->setAxisAlignment( osgText::Text::SCREEN );
	m_rText->setColor( osg::Vec4(1,1,1,1) );
	m_rText->setText( "" );	
	m_rText->setPosition( osg::Vec3( 6, m_h+4, 0) );
	m_rText->setDataVariance( osg::Object::DYNAMIC );
	m_rText->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	m_rText->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	m_rGeode->addDrawable( m_rText.get() );

	// make the cursor blink
	m_rGeode->addUpdateCallback( new CCursorUpdate( this ) );

	// manage input
	m_rViewer->addEventHandler( new CSulConsoleInputHandler( this ) );
	
	CreateTextObjects( m_iTextObjs );

	CSulAnimationPathFunctor<CSulConsoleDisplay>* pFunctor = new CSulAnimationPathFunctor<CSulConsoleDisplay>(this, &CSulConsoleDisplay::BlindCheck);
	m_rAniPathCallback->WantCallback( pFunctor );
}

void CSulConsoleDisplay::SetBgColor( float r, float g, float b, float a )
{
	m_rQuad->setColor( r, g, b, a );
}

const osg::Vec4& CSulConsoleDisplay::GetBgColor()
{
	return m_rQuad->getColor( 0 );
}

void CSulConsoleDisplay::GetBgColor( float& r, float& g, float& b, float& a )
{
	osg::Vec4 c = m_rQuad->getColor( 0 );
	r = c.r();
	g = c.g();
	b = c.b();
	a = c.a();
}

void CSulConsoleDisplay::SetActivateKey( sigma::int32 iKey )
{
	m_iActivateKey = iKey;
}

int CSulConsoleDisplay::GetActivateKey()
{
	return m_iActivateKey;
}

void CSulConsoleDisplay::BlindCheck()
{
	if ( m_bShow )
	{
		m_bActive = true;
	}
}

void CSulConsoleDisplay::Show( bool bShow )
{
	if ( bShow==m_bShow )
	{
		return;
	}

	m_bShow = bShow;
	m_rAniPathCallback->SetMode( m_bShow?CSulAnimationPathCallback::MODE_PLAY_FORWARD:CSulAnimationPathCallback::MODE_PLAY_REVERSE );
}

bool CSulConsoleDisplay::IsVisible()
{
	return m_bShow;
}

void CSulConsoleDisplay::SetActive( bool bActive )
{
	if ( bActive==m_bActive )
	{
		return;
	}

	m_bActive = bActive;
}

osg::Node* CSulConsoleDisplay::GetNode()
{
	return m_rNode.get();
}

void CSulConsoleDisplay::CreateTextObjects( sigma::uint32 iCount )
{
	// note:: might have to removeDrawables from geode first
	m_vecTextObject.clear();
	
	sigma::uint32 i;
	for ( i=0; i<iCount; i++ )
	{
		CSulString s;
		s.Format( "line %d", i );

		osgText::Text* pText = new osgText::Text;
		pText->setCharacterSize( m_fCharSize );
		pText->setFont( "Tempest.ttf" );
		pText->setAxisAlignment( osgText::Text::SCREEN );
		pText->setColor( osg::Vec4(1,1,1,1) );
		pText->setText( "" );	
		pText->setPosition( osg::Vec3( 6, m_h+4+m_iSpacingV*(i+1), 0) );
		pText->setDataVariance( osg::Object::DYNAMIC );
		pText->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
		pText->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		m_rGeode->addDrawable( pText );
		m_vecTextObject.push_back( pText );
	}
}

void CSulConsoleDisplay::SetCursor( const CSulString& s )
{
	m_sCursor = s;
	UpdateText();
}

void CSulConsoleDisplay::DisplayText( const CSulString& sText )
{
	// save history of commands
	m_vecTextBuffer.insert( m_vecTextBuffer.begin(), sText );
	UpdateTextObjects();

	// reset marker position
	m_iMarkerPos = -1;
}

void CSulConsoleDisplay::ExecuteCommand( const CSulString& sCmd )
{
	// save history of commands
	m_vecTextBuffer.insert( m_vecTextBuffer.begin(), sCmd );
	m_vecTextHistory.insert( m_vecTextHistory.begin(), sCmd );

	UpdateTextObjects();

	// notify anyone that wants the command
	VECTOR_CONSOLEFUNCTOR::iterator i;
	for ( i=m_vecConsoleFunctor.begin(); i!=m_vecConsoleFunctor.end(); ++i )
	{
		CSulConsoleFunctorBase* pFunctor = i->get();
		pFunctor->Call( sCmd );
	}

	// reset marker position
	m_iMarkerPos = -1;
}

void CSulConsoleDisplay::WantCommand( CSulConsoleFunctorBase* pFunctor )
{
	m_vecConsoleFunctor.push_back( pFunctor );
}

void CSulConsoleDisplay::UpdateText()
{
	CSulString s;
	s = ">";
	s+= m_sText;
	s+= m_sCursor;
	m_rText->setText( s );
}

void CSulConsoleDisplay::MarkerUp()
{
	if ( (m_iMarkerPos+1)<(sigma::int32)m_vecTextHistory.size() )
	{
		++m_iMarkerPos;
		m_sText = m_vecTextHistory[m_iMarkerPos];
		UpdateText();
	}
}

void CSulConsoleDisplay::MarkerDown()
{
	if ( m_iMarkerPos>-1 )
	{
		--m_iMarkerPos;
		if ( m_iMarkerPos==-1 )
		{
			m_sText = "";
		}
		else
		{
			m_sText = m_vecTextHistory[m_iMarkerPos];
		}
		
		UpdateText();
	}
}

void CSulConsoleDisplay::UpdateTextObjects()
{
	VECTOR_TEXT::iterator i;
	sigma::uint32 iBufSize = m_vecTextBuffer.size();

	// updates the text from bottom to top
	sigma::uint32 iCount = 0;
	for ( i=m_vecTextObject.begin(); i!=m_vecTextObject.end(); i++ )
	{
		if ( (iCount+m_iScrollPos)<iBufSize )
		{
			i->get()->setText( m_vecTextBuffer[iCount+m_iScrollPos] );
			++iCount;
		}
	}
}

void CSulConsoleDisplay::ScrollUp()
{
	// calc how many positions we can scroll up
	sigma::int32 i = m_vecTextBuffer.size()-m_vecTextObject.size();

	if ( m_iScrollPos<i )
	{
		m_iScrollPos++;
		UpdateTextObjects();
	}
}

void CSulConsoleDisplay::ScrollDown()
{
	if ( m_iScrollPos>0 )
	{
		m_iScrollPos--;
		UpdateTextObjects();
	}
}

void CSulConsoleDisplay::PageUp()
{
	// calc how many positions we can scroll up
	sigma::int32 i = m_vecTextBuffer.size()-m_vecTextObject.size();
	sigma::int32 iCountView = m_vecTextObject.size();

	if ( (m_iScrollPos+iCountView)<i )
	{
		m_iScrollPos+= iCountView;
		UpdateTextObjects();
	}
}

void CSulConsoleDisplay::PageDown()
{
	sigma::int32 iCountView = m_vecTextObject.size();
	if ( (m_iScrollPos-iCountView)>0 )
	{
		m_iScrollPos-= iCountView;
	}
	else
	{
		m_iScrollPos = 0;
	}
	
	UpdateTextObjects();
}

void CSulConsoleDisplay::Input( sigma::int32 key, sigma::int32 iMod )
{
	if ( key==m_iActivateKey )
	{
		Show( !m_bShow );
		return;
	}

	if ( !m_bActive )
	{
		return;
	}

	// keys to ignore
	if ( key==osgGA::GUIEventAdapter::KEY_Left ||
		key==osgGA::GUIEventAdapter::KEY_Right )
	{
		return;
	}

	if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Page_Up )
	{
		PageUp();
		return;
	}

	if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Page_Down )
	{
		PageDown();
		return;
	}

	// check for ctrl-up
	if ( (iMod==osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL || iMod==osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL) && key==osgGA::GUIEventAdapter::KEY_Up )
	{
		ScrollUp();
		return;
	}

	// check for ctrl-down
	if ( (iMod==osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL || iMod==osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL) && key==osgGA::GUIEventAdapter::KEY_Down )
	{
		ScrollDown();
		return;
	}

	if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Up )
	{
		MarkerUp();
		return;
	}

	if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Down )
	{
		MarkerDown();
		return;
	}

	if ( iMod==0 && key==osgGA::GUIEventAdapter::KEY_Return )
	{
		ExecuteCommand( m_sText );
		m_sText = "";
		return;
	}

	if ( key==osgGA::GUIEventAdapter::KEY_BackSpace )
	{
		m_sText = m_sText.substr(0, m_sText.size()-1);
		UpdateText();
		return;
	}

	if (	key!=osgGA::GUIEventAdapter::KEY_Shift_L && 
			key!=osgGA::GUIEventAdapter::KEY_Shift_R && 
			key!=osgGA::GUIEventAdapter::KEY_Control_L &&
			key!=osgGA::GUIEventAdapter::KEY_Control_R 
		)
	{
		m_sText += key;
		UpdateText();
	}
}
