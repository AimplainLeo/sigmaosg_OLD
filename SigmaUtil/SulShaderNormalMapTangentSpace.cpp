// SulShaderNormalMapTangentSpace.cpp

#include "stdafx.h"
#include "SulShaderNormalMapTangentSpace.h"
#include <osg/geode>
#include <osg/geometry>
#include <osgUtil/TangentSpaceGenerator>

class TangentSpaceVisitor: public osg::NodeVisitor 
{
public:
    // constructor
    TangentSpaceVisitor(const std::string &vertexAttributeName, osg::Program *shaderProgram = NULL, int tangentVertexAttribNumber = 6): 
    NodeVisitor(NodeVisitor::TRAVERSE_ALL_CHILDREN),
    mVertexAttributeName(vertexAttributeName),
    mShaderProgram(shaderProgram),
    mTangentVertexAttribNumber(tangentVertexAttribNumber)
    {
    }

    // apply - called for each node
    void apply(osg::Geode& geode)
    {
		for (unsigned i = 0; i < geode.getNumDrawables(); ++i) 
		{
			osg::Geometry* pGeom = dynamic_cast<osg::Geometry* >(geode.getDrawable(i));
			if (pGeom != NULL) 
			{
				// if the tangents are not already set, then put the tangents on.
				if (!pGeom->getVertexAttribArray(mTangentVertexAttribNumber))
				{
					osg::ref_ptr<osgUtil::TangentSpaceGenerator> tsg = new osgUtil::TangentSpaceGenerator;
	
					tsg->generate(pGeom, 0);
					pGeom->setVertexAttribArray(mTangentVertexAttribNumber, tsg->getTangentArray());
					pGeom->setVertexAttribBinding(mTangentVertexAttribNumber, osg::Geometry::BIND_PER_VERTEX );

					if (mShaderProgram != NULL)
					{
						mShaderProgram->addBindAttribLocation(mVertexAttributeName, mTangentVertexAttribNumber);
					}
/*
					//DEBUG
					{
						//IUASSERT(CIuGeometryValidator::Validate(pGeom) == true);
						if(!CIuGeometryValidator::Validate(pGeom))
						{
							//std::cout << "Validating " << geode.getName() << std::endl;
							LOG_WARNING("Geode, " + geode.getName() + ", contains degenerate triangles. Tangent space normal mapping will suffer.");
						}
					}
*/
				}
			}
		}

		NodeVisitor::apply(geode);
    }

private: 
    std::string mVertexAttributeName;
    osg::Program *mShaderProgram;
    int mTangentVertexAttribNumber;
};


CSulShaderNormalMapTangentSpace::CSulShaderNormalMapTangentSpace( osg::Node* pNode, Sigma::int32 indexLight, osg::Program* pProg ) :
CSulShaderBase( pNode )
{
	if ( !pProg )
	{
		addShaderFrag( "shaders/normalmap_tangentspace.frag" );
		addShaderVert( "shaders/normalmap_tangentspace.vert" );

		osg::Uniform* uniformIndexLight = new osg::Uniform( osg::Uniform::INT, "indexLight" );
		uniformIndexLight->set( indexLight );
		m_rSS->addUniform( uniformIndexLight );

		osg::Uniform* uniformColorMap = new osg::Uniform( osg::Uniform::SAMPLER_2D, "colorMap" );
		uniformColorMap->set( 0 );		
		m_rSS->addUniform( uniformColorMap );

		osg::Uniform* uniformNormalMap = new osg::Uniform( osg::Uniform::SAMPLER_2D, "normalMap" );
		uniformNormalMap->set( 1 );		
		m_rSS->addUniform( uniformNormalMap );
	}

	
	osg::ref_ptr<TangentSpaceVisitor> visitor = new TangentSpaceVisitor( "vTangent", m_rProg, 6 );
	pNode->accept( *visitor.get() );
}