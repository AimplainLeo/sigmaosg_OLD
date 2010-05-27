// input.h

#ifndef __INPUT_H__
#define __INPUT_H__

#include <SigmaUtil/SulGrass.h>
#include <SigmaUtil/SulGenerateHeightTexture.h>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

class CInput : public osgGA::GUIEventHandler 
{
public:
	CInput( osg::Node* pTerrain, osg::Group* pRoot, CSulGrass* pGrass )
	{
		m_rTerrain = pTerrain;
		m_rRoot = pRoot;
		m_rGrass = pGrass;
	}

	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( !pViewer )
        {
            return false;
        }

        if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
        {
			// save
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_F1 )
            {
				m_genTex = new CSulGenerateHeightTexture;

				int grid_size = m_rGrass->getGridSize();

				m_genTex->create( 
					m_rTerrain,
					m_rRoot,
					0, 0,
					1024,			// texture size
					grid_size		// grid size in meters
				);

                return true; // return true, event handled
            }

			// load
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_F2 )
            {
				// use the generated texture from the F1
				m_rGrass->setHeightTexture( m_genTex->getTexture() );

                return true; // return true, event handled
            }
        }

        return false;
    }

private:
	osg::ref_ptr<CSulGenerateHeightTexture>	m_genTex;
	osg::ref_ptr<osg::Node>					m_rTerrain;
	osg::ref_ptr<osg::Group>				m_rRoot;
	osg::ref_ptr<CSulGrass>					m_rGrass;
};

#endif // __INPUT_H__