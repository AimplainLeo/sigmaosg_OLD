// SulCalcBoundingBox.h

#ifndef __SULCALCBOUNDINGBOX_H__
#define __SULCALCBOUNDINGBOX_H__

#include <osg/NodeVisitor>
#include <osg/BoundingBox>
#include <osg/BoundingSphere>
#include <osg/MatrixTransform>
#include <osg/Billboard>
 
///////////////////////////////////////////////////////////////////////////////
//
//	DEPRECATED!!!!!!!!!!!
//	USE THIS INSTEAD:
//		osg::ComputeBoundsVisitor computeBoundsVisitor;
//		pNode->accept(computeBoundsVisitor);
//		osg::BoundingBox b = computeBoundsVisitor.getBoundingBox();
//
// How to use:
//
//	CSulCalcBoundingBox b;
//	node->accept( b );
//	osg::BoundingBox bounds = b.bbox();
// 
///////////////////////////////////////////////////////////////////////////////

class  CSulCalcBoundingBox : public osg::NodeVisitor 
{
public:
 	CSulCalcBoundingBox( bool bTransformToWorld=false ) : 
		NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ),
		m_bTransformToWorld( bTransformToWorld )
	{
		m_transformMatrix.makeIdentity();
	}
       
    virtual ~CSulCalcBoundingBox() {}
 
    virtual void apply( osg::Geode &geode ) 
	{
        osg::BoundingBox bbox;

		osg::Matrix m;

		if ( m_bTransformToWorld )
		{
			m = osg::computeLocalToWorld(getNodePath());
		}
        
        // update bounding box for each drawable
        for(  unsigned int i = 0; i < geode.getNumDrawables(); ++i )
		{
			// expand the overall bounding box
            bbox.expandBy( geode.getDrawable( i )->getBound());
		}
 
        // transform corners by current matrix
        osg::BoundingBox bboxTrans;
        
        for( unsigned int i = 0; i < 8; ++i ) 
		{
            osg::Vec3 xvec = bbox.corner( i ) * m;
            bboxTrans.expandBy( xvec );
		}
 
        // update the overall bounding box size
        m_boundingBox.expandBy( bboxTrans );

		if ( m_boundingBox.xMax()-m_boundingBox.xMin()>1024 ) 
		{
			int a = 0;
		}
 
        // continue traversing through the graph
        traverse( geode );
	}
 
    virtual void apply( osg::Billboard &node )
	{
	    //    Handle nodes of the type osg::Billboard

        // important to handle billboard so that its size will
        // not affect the geode size continue traversing the graph
        traverse( node );
    } 
    
    osg::BoundingBox &getBoundBox() { return m_boundingBox; }  
 
protected :
 
    osg::BoundingBox	m_boundingBox;          // the overall resultant bounding box
    osg::Matrix			m_transformMatrix;      // the current transform matrix
	bool				m_bTransformToWorld;
};  

#endif // __SULCALCBOUNDINGBOX_H__