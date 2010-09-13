// SulGuiCompEditModeVisitor.h

#ifndef __SULGUICOMPEDITMODEVISITOR_H__
#define __SULGUICOMPEDITMODEVISITOR_H__

class CSulGuiCompEditModeVisitor : public osg::NodeVisitor
{
public:
	CSulGuiCompEditModeVisitor( bool bEdit ) : 
	  osg::NodeVisitor( TRAVERSE_ALL_CHILDREN )
	{
		m_bEdit = bEdit;
	}

	virtual void apply( osg::Node& node )
	{
		CSulGuiComp* p = dynamic_cast<CSulGuiComp*>(&node);
		if ( p )
		{
			p->setEditMode( m_bEdit );
		}

		traverse( node );
	}

private:
	bool m_bEdit;
};

#endif // __SULGUICOMPEDITMODEVISITOR_H__