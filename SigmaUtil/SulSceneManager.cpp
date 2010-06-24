// SulSceneManager.cpp

#include "stdafx.h"
#include "SulSceneManager.h"
#include "SulSceneManagerXml.h"

bool CSulSceneManager::Load( const CSulString& sXml )
{
	osg::ref_ptr<CSulSceneManagerXml> rXml = new CSulSceneManagerXml( this );
	if ( !rXml->Load( sXml.c_str() ) )
	{
		return false;
	}

	return true;
}

bool CSulSceneManager::Create( const CSulString& sName, osg::Group* pParent, osg::Node* pUseThisNode )
{
	MAP_NODE::iterator i;

	i = m_mapNode.find( sName );
	if ( i==m_mapNode.end() )
	{
		if ( pUseThisNode )
		{
			m_mapNode[sName] = pUseThisNode;
		}
		else
		{
			m_mapNode[sName] = new osg::Group;
		}

		m_mapNode[sName]->setName( sName );

		if ( pParent )
		{
			pParent->addChild( m_mapNode[sName].get() );
		}

		return true;
	}

	osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::Create -> ["<<sName<<"] already exists, must be unique!"<<std::endl;
	return false;
}

osg::Node* CSulSceneManager::Get( const CSulString& sName )
{
	MAP_NODE::iterator i;

	i = m_mapNode.find( sName );
	if ( i!=m_mapNode.end() )
	{
		return i->second.get();
	}

	osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::Get -> could not find ["<<sName<<"]"<<std::endl;
	return 0;
}

bool CSulSceneManager::Add( const CSulString& sName, osg::Group* pParent, osg::Node* pNode )
{
	MAP_NODE::iterator i;

	i = m_mapNode.find( sName );
	if ( i!=m_mapNode.end() )
	{
		// sName already exists (must be unique)
		osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::Add -> ["<<sName<<"] already exists, must be unique!"<<std::endl;
		return false;
	}

	m_mapNode[sName] = pNode;
	m_mapNode[sName]->setName( sName );

	if ( pParent )
	{
		pParent->addChild( pNode );
	}

	return true;
}

bool CSulSceneManager::Remove( const CSulString& sName, osg::Node* pNode )
{
	MAP_NODE::iterator i;

	i = m_mapNode.find( sName );
	if ( i!=m_mapNode.end() )
	{
		m_mapNode.erase( i );
		return true;
	}

	osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::Remove -> could not find ["<<sName<<"]"<<std::endl;
	return false;
}

bool CSulSceneManager::AddToGroup( const CSulString& sGroupName, osg::Node* pNode )
{
	osg::Group* pGroup = dynamic_cast<osg::Group*>(Get(sGroupName));
	if ( !pGroup )
	{
		osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::AddToGroup -> group ["<<sGroupName<<"] does NOT exist!"<<std::endl;
		return false;
	}

	pGroup->addChild( pNode );

	return true;
}

bool CSulSceneManager::RemoveFromGroup( const CSulString& sGroupName, osg::Node* pNode )
{
	osg::Group* pGroup = dynamic_cast<osg::Group*>(Get(sGroupName));
	if ( !pGroup )
	{
		osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::AddFromGroup -> group ["<<sGroupName<<"] does NOT exist!"<<std::endl;
		return false;
	}

	pGroup->removeChild( pNode );

	return true;
}

void CSulSceneManager::ResetAllGroups()
{
	MAP_NODE::iterator i;
	
	for ( i=m_mapNode.begin(); i!=m_mapNode.end(); ++i )
	{
		osg::Group* pGroup = dynamic_cast<osg::Group*>(i->second.get());
		if ( pGroup && pGroup->getNumChildren() )
		{
			// make sure the first child is not a part of the layout!
			if ( !IsNodeInLayout( pGroup->getChild( 0 ) ) )
			{
				// delete all children
				pGroup->removeChild( 0, pGroup->getNumChildren() );
			}
		}
	}
}

bool CSulSceneManager::AddStateSet( const CSulString& sName, osg::StateSet* pStateSet )
{
	if ( m_mapStateSet.find( sName ) != m_mapStateSet.end() )
	{
		osg::notify(osg::WARN)<<"WARNING: CSulSceneManager::AddStateSet -> ["<<sName<<"] already exists, must be unique!"<<std::endl;
		return false;
	}

	m_mapStateSet.insert( MAP_STATESET::value_type(sName, pStateSet) );

	return true;
}

osg::StateSet* CSulSceneManager::GetStateSet( const CSulString& sName )
{
	MAP_STATESET::const_iterator it = m_mapStateSet.find( sName );
	if ( it != m_mapStateSet.end() )
	{
		return it->second.get();
	}

	return 0;
}

bool CSulSceneManager::IsNodeInLayout( osg::Node* pNode )
{
	MAP_NODE::iterator i;
	
	for ( i=m_mapNode.begin(); i!=m_mapNode.end(); ++i )
	{
		if ( i->second.get()==pNode )
		{
			return true;
		}
	}

	return false;
}

void CSulSceneManager::addAttribute( const CSulString& sName, osg::StateAttribute* pAttr )
{
	m_mapStateAttribute[sName] = pAttr;
}

osg::StateAttribute* CSulSceneManager::getAttribute( const CSulString& sName )
{
	Sigma::MAP_STRING_STATEATTRIBUTE::iterator iFound;

	iFound = m_mapStateAttribute.find( sName );
	if ( iFound!=m_mapStateAttribute.end() )
	{
		return iFound->second;
	}

	return 0;
}


