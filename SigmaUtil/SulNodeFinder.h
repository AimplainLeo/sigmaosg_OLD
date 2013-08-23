// SulNodeFinder.h

#ifndef __SULNODEFINDER_H__
#define __SULNODEFINDER_H__

#include <osg/node>

class CSulNodeFinder
{
public:
	static osg::Node* findNodeByName( osg::Node* pNode, const std::string& sName );

	template< typename T>
	static T* findNodeByType( osg::Node* searchFrom )
	{
	    T* found = dynamic_cast<T*>(searchFrom);
		if ( found )
			return found;

		osg::Group* group = searchFrom->asGroup();
		if ( group )
		{
			for ( unsigned int i=0; i<group->getNumChildren(); i++ )
			{
				T* found = CSulNodeFinder::findNodeByType<T>( group->getChild(i) );
				if ( found )
					return found;
			}
		}

		return 0;
	}
};

#endif // __SULNODEFINDER_H__