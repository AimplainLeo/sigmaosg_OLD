// SulGenGrassUpdateCallback.h

#ifndef __SULGENGRASSUPDATECALLBACK_H__
#define __SULGENGRASSUPDATECALLBACK_H__

#include "SulGenerateHeightTexture.h"

class CSulGenGrassUpdateCallback : public osg::NodeCallback
{
public:
	CSulGenGrassUpdateCallback( CSulGrass* pGrass, osg::Group* pRoot, osg::Node* pTerrain, const osg::Vec3& pos )
	{
		pGrass->getGeode()->setUpdateCallback( this );

		m_rGrass = pGrass;
		m_rRoot = pRoot;
		m_rTerrain = pTerrain;
		count = 0;

		m_genTex = new CSulGenerateHeightTexture;

		int grid_size = m_rGrass->getGridSize();

		m_genTex->create( 
			m_rTerrain,
			m_rRoot,
			pos.x(), pos.y(),	// position
			1024,				// texture size
			grid_size			// grid size in meters
		);

	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		count++;

		if ( count==4 )
		{
			m_rGrass->setHeightTexture( m_genTex->getTexture() );
		}

		// traverse subtree
        traverse(node,nv);
	}

	osg::ref_ptr<CSulGenerateHeightTexture> m_genTex;
	osg::ref_ptr<CSulGrass>					m_rGrass;
	osg::ref_ptr<osg::Group>				m_rRoot;
	osg::ref_ptr<osg::Node>					m_rTerrain;

	int count;
};

#endif // __SULGENGRASSUPDATECALLBACK_H__