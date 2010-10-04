// SulGenNodeMaskVisitor.h

#ifndef __SULGENNODEMASKVISITOR_H__
#define __SULGENNODEMASKVISITOR_H__

#include "SulTypes.h"
#include <osg/geode>

class CSulGenNodeMaskVisitor : public osg::NodeVisitor
{
private:
	typedef std::vector< CSulString >	VEC_STRING;

public:
 	CSulGenNodeMaskVisitor( VEC_STRING vecIgnoreNodeList ) : 
		NodeVisitor( NodeVisitor::TRAVERSE_ALL_CHILDREN ) 
	{
		m_vecIgnoreNodeList = vecIgnoreNodeList;
	}

    virtual void apply( osg::Node& n ) 
	{
		VEC_STRING::iterator iFound;

		iFound = find( m_vecIgnoreNodeList.begin(), m_vecIgnoreNodeList.end(), n.getName() );
		if ( iFound!=m_vecIgnoreNodeList.end() )
		{
			n.setNodeMask( 0 );
		}

		// continue traversing through the graph
        traverse( n );
	}

private:
	VEC_STRING	m_vecIgnoreNodeList;
};

#endif // __SULGENNODEMASKVISITOR_H__