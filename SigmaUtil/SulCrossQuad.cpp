// SulCrossQuad.cpp

#include "stdafx.h"
#include "SulCrossQuad.h"
#include "SulRTT.h"
#include "SulGeomQuad.h"
#include "SulBB.h"
#include "SulGeomAxis.h"
#include "SulJsonArray.h"
#include "SulJsonUtil.h"
#include "SulJsonObject.h"
#include "SulJsonString.h"
#include <osg/computeboundsvisitor>

class CDataLod : public osg::Referenced
{
public:
	CDataLod( sigma::uint32 texW, sigma::uint32 texH, sigma::uint32 minRange, sigma::uint32 maxRange ) :
	m_texW( texW ),
	m_texH( texH ),
	m_minRange( minRange ),
	m_maxRange( maxRange )
	{}

public:
	sigma::uint32 m_texW;
	sigma::uint32 m_texH;
	sigma::uint32 m_minRange;
	sigma::uint32 m_maxRange;
};

typedef std::map< sigma::uint32, osg::ref_ptr<CDataLod> > MAP_DATALOD;

static MAP_DATALOD m_mapDataLod;

CSulCrossQuad::CSulCrossQuad( osgViewer::Viewer* pViewer, osg::Node* pRender, osg::BoundingBox* pBB, const CSulString& sJson, bool bShowLodBB ) :
osg::Group(),
m_rViewer( pViewer ),
m_rRender( pRender ),
m_sJson( sJson ),
m_pBB( pBB ),
m_bShowLodBB( bShowLodBB )
{
	CSulJsonArray* jsonArray = 0;

	if ( !m_sJson.empty() )
	{
		CSulJsonUtil json;
		jsonArray = json.create( m_sJson )->asArray();

		sigma::uint32 count = jsonArray->getCount();

		for ( sigma::uint32 i=0; i<count; i++ )
		{
			CSulJsonObject* pObj = jsonArray->get( i )->asObject();

			sigma::uint32 level		= pObj->get( "level" )->asString()->get().asUint32();
			sigma::uint32 texW		= pObj->get( "texW" )->asString()->get().asUint32();
			sigma::uint32 texH		= pObj->get( "texH" )->asString()->get().asUint32();
			sigma::uint32 minRange	= pObj->get( "minRange" )->asString()->get().asUint32();
			sigma::uint32 maxRange	= pObj->get( "maxRange" )->asString()->get().asUint32();

			m_mapDataLod[level] = new CDataLod(
				texW,
				texH,
				minRange,
				maxRange
			);
		}
	}

	process();
}

osg::ref_ptr<osg::Texture2D> CSulCrossQuad::createTextureFromOrthoView2( sigma::uint32 texW, sigma::uint32 texH, osg::Node* pRenderMe, ORTHOVIEW eOrthoView, osg::Vec3 pos, float far )
{
	osg::ref_ptr<CSulRTT> rtt = new CSulRTT( texW, texH, CSulRTT::SETUP_ORTHO_FRONT );
	rtt->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );

	if ( m_bShowLodBB )
		rtt->setClearColor( osg::Vec4(1,0,0,1) );
	else
		rtt->setClearColor( osg::Vec4(0,0,0,0) );

	rtt->addChild( pRenderMe );

	// bounding box
	osg::BoundingBox bb;
	if ( m_pBB )
	{
		bb = *m_pBB;
	}
	else
	{
		osg::ComputeBoundsVisitor* tmp = new osg::ComputeBoundsVisitor;
		pRenderMe->accept( *tmp );
		bb = tmp->getBoundingBox();
	}

	// force camera into ortha and look at trees
	float l;
	float r;
	float t = bb.zMax();
	float b = bb.zMin();
	
	osg::Matrix m;
	switch ( eOrthoView )
	{
		case FRONT:
			{
				float mm[16] =
				{
					 1, 0,  0, 0,		// x,y,z
					 0, 0, -1, 0,
					 0, 1,  0, 0,
					 0.0f, 0.0f, pos.y()-far/2.0f, 1
				};
				
				m.set( mm );

				l = bb.xMin();
				r = bb.xMax();
				
				b = bb.zMin();
				t = bb.zMax();
			}
			break;

		case LEFT:
			{
				float mm[16] =
				{
					  0, 0, -1, 0,		// x,y,z
					  -1, 0,  0, 0,
					  0, 1,  0, 0,
					  0, 0,  pos.x()-far/2.0f, 1
				};

				m.set( mm );

				l =- bb.yMin();
				r = -bb.yMax();

				t = bb.zMax();
				b = bb.zMin();

			}
			break;
	}

	rtt->setViewMatrix( m );

	float near = 0.0f;
	rtt->setProjectionMatrixAsOrtho( l, r, b, t, near, far  );

	osg::ref_ptr<osg::Image> img = new osg::Image;
	rtt->attach( osg::Camera:: COLOR_BUFFER, img );

	m_rViewer->getSceneData()->asGroup()->addChild( rtt );
	m_rViewer->frame();
	m_rViewer->getSceneData()->asGroup()->removeChild( rtt );	

	return rtt->getTexture();
}

osg::Node* CSulCrossQuad::createQuad( sigma::uint32 texW, sigma::uint32 texH, const osg::Vec3& pos, float l, float h, float far, float near, CSulGeomQuad::EPLANE ePlane )
{
	CSulGeomQuad* p = new CSulGeomQuad( pos, l, h, ePlane );
	p->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	ORTHOVIEW ov = FRONT;
	if ( ePlane==CSulGeomQuad::PLANE_YZ )
		ov = LEFT;

	osg::Texture2D* pTex = createTextureFromOrthoView2( texW, texH, m_rRender, ov, pos, far );
	p->setTexture( pTex );

	return p;
}

void CSulCrossQuad::process()
{
	osg::BoundingBox bb;

	if ( m_pBB )
	{
		bb = *m_pBB;
	}
	else
	{
		osg::ref_ptr<osg::ComputeBoundsVisitor> tmp = new osg::ComputeBoundsVisitor;
		m_rRender->accept( *tmp );
		bb = tmp->getBoundingBox();
	}

	float l = bb.xMax()-bb.xMin();
	float d = bb.yMax()-bb.yMin();
	float h = bb.zMax()-bb.zMin();

	sigma::uint32 numLevels = m_mapDataLod.size();

	for ( sigma::uint32 i=1; i<numLevels+1; i++ )
	{
		float min = m_mapDataLod[i]->m_minRange;
		float max_range = m_mapDataLod[i]->m_maxRange;
		sigma::uint32 texW = m_mapDataLod[i]->m_texW;
		sigma::uint32 texH = m_mapDataLod[i]->m_texH;

		// number of slices
		sigma::uint32 n = (sigma::uint32)pow( 2.0f, (float)(numLevels+1)-i)-1;

		float subL = l / n;
		float subD = d / n;

		float near = 0.0f;
		float far = subD;

		osg::ref_ptr<osg::Group> group = new osg::Group;

		// step through each slice
		for ( sigma::uint32 ii=0; ii<n; ii++ )
		{ 
			osg::Vec3 pos1( (bb.xMax()+bb.xMin())/2.0f, bb.yMin() + subD/2.0f +subD*(ii), (bb.zMax()+bb.zMin())/2.0f );
			osg::Node* pQuad1 = createQuad( texW, texH, pos1, l, h, far, near, CSulGeomQuad::PLANE_XZ );
			group->addChild( pQuad1 );

			osg::Vec3 pos2( bb.xMin() + subD/2.0f +subD*ii, (bb.yMax()+bb.yMin())/2.0f, (bb.zMax()+bb.zMin())/2.0f );
			osg::Node* pQuad2 = createQuad( texW, texH, pos2, l, h, far, near, CSulGeomQuad::PLANE_YZ );
			group->addChild( pQuad2 );
		}

		osg::ref_ptr<osg::LOD> lod = new osg::LOD;
		lod->setRange( 0, min, max_range );
		lod->addChild( group );
		addChild( lod );

		if ( m_bShowLodBB )
		{
			CSulBB* pDrawBB = new CSulBB( lod );
			addChild( pDrawBB );
		}
	}

}
