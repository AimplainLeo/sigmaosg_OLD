// SulSceneManager.h

#ifndef __SULSCENEMANAGER_H__
#define __SULSCENEMANAGER_H__

#include "SulExport.h"
#include "SulString.h"
#include <osg/Referenced>

class SUL_EXPORT CSulSceneManager : public osg::Referenced
{
private:
	typedef std::map<CSulString, osg::ref_ptr<osg::Node> >	MAP_NODE;
	typedef std::map<CSulString, osg::ref_ptr<osg::StateSet> >	MAP_STATESET;

public:

	////////////////////////////////////////////////////////////////////////////////////
	// use to manage scene layout
	////////////////////////////////////////////////////////////////////////////////////

	virtual bool	Load( const CSulString& sXml );

	virtual bool	Create( const CSulString& sName, osg::Group* pParent, osg::Node* pUseThisNode=0 );
	osg::Node*		Get( const CSulString& sName );

	virtual bool	Add( const CSulString& sName, osg::Group* pParent, osg::Node* pNode );
	virtual bool	Remove( const CSulString& sName, osg::Node* pNode );

	////////////////////////////////////////////////////////////////////////////////////
	// use to manage objects in the scene layout
	////////////////////////////////////////////////////////////////////////////////////

	virtual bool	AddToGroup( const CSulString& sGroupName, osg::Node* pNode );
	virtual bool	RemoveFromGroup( const CSulString& sGroupName, osg::Node* pNode );

	virtual void	ResetAllGroups();

	////////////////////////////////////////////////////////////////////////////////////
	// use to manage statesets in the scene layout
	////////////////////////////////////////////////////////////////////////////////////
	
	virtual bool	AddStateSet( const CSulString& sName, osg::StateSet* pStateSet );
	osg::StateSet*	GetStateSet( const CSulString& sName );

	////////////////////////////////////////////////////////////////////////////////////
	// use to manage stateattributes in the scene layout
	////////////////////////////////////////////////////////////////////////////////////

	virtual	void			addAttribute( const CSulString& sName, osg::StateAttribute* pAttr );
	osg::StateAttribute*	getAttribute( const CSulString& sName );

	////////////////////////////////////////////////////////////////////////////////////
	// use to manage node masks
	////////////////////////////////////////////////////////////////////////////////////

	sigma::uint32	calcCullMask( const std::string& maskNames );
	sigma::uint32	getOrCreateMaskValue(const CSulString& name);

private:
	bool			IsNodeInLayout( osg::Node* pNode );

private:
	MAP_NODE							m_mapNode;
	MAP_STATESET						m_mapStateSet;
	sigma::MAP_STRING_STATEATTRIBUTE	m_mapStateAttribute;
	std::map<std::string,int>			m_mapMask;
};

#endif // __SULSCENEMANAGER_H__