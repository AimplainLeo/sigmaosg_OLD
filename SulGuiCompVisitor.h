// SulGuiCompVisitor.h

#ifndef __SULGUICOMPVISITOR_H__
#define __SULGUICOMPVISITOR_H__

class CSulGuiCompVisitor : public osg::NodeVisitor
{
public:
	CSulGuiCompVisitor( const CSulString& id ) :
	  osg::NodeVisitor( TRAVERSE_ALL_CHILDREN )
	{
		m_id = id;
		m_foundNode = 0;
	}

	CSulGuiComp* getFound()
	{
		return m_foundNode;
	}

	virtual void apply( osg::Node& node )
	{
		CSulGuiComp* p = dynamic_cast<CSulGuiComp*>(&node);
		if ( p )
		{
			if ( p->getId()==m_id )
			{
				m_foundNode = p;
				return;
			}
		}

		traverse( node );
	}

private:
	CSulString	m_id;
	CSulGuiComp*	m_foundNode;
};

#endif // __SULGUICOMPVISITOR_H__