// SulNode.h

#ifndef __SULNODE_H__
#define __SULNODE_H__

/*
	NOTE:

	This is a generic node and has nothing to do with osg::Node. This is a utility node where certain types of code needs a form of linked-lists

*/

class CSulNode : public osg::Referenced
{
public:
	typedef std::vector< osg::ref_ptr<CSulNode> > VEC_NODE;

public:
	CSulNode();

	void					setParent( class CSulNodeGroup* parent );
	class CSulNodeGroup*	getParent();

private:
	class CSulNodeGroup*	m_parent;
};



#endif // __SULNODE_H__