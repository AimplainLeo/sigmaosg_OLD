// SulXmlWriter.cpp

#include "stdafx.h"
#include "SulXmlWriter.h"
#include "SulXmlAttr.h"
#include "SulFile.h"

CSulXmlWriter::CSulXmlWriter( const char* pszRootName )
{
	 m_strRootName = pszRootName;
	 m_pAttrRoot = elementStart( m_strRootName.c_str() );
}

CSulXmlWriter::~CSulXmlWriter()
{
}

CSulXmlAttr* CSulXmlWriter::GetRootAttr()
{
	return m_pAttrRoot;
}

bool CSulXmlWriter::Save( const char* pszXmlFile )
{
	osg::ref_ptr<CSulFile> rFile = new CSulFile;

//	FILE*	pFile;
//	int		iRet;
//	errno_t	err;

	// this should be end on root
	elementEnd();

	bool bRet = rFile->Open( pszXmlFile, CSulFile::MODE_WRITE );
	if ( !bRet )
	{
		// should output a warning here
		return false;
	}
	/*
	err = fopen_s( &pFile, pszXmlFile, "w" );
	assert( !err );
	*/

	// save xml header
	bRet = rFile->Print( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" );
	if ( !bRet )
	{
		rFile->Close();
		return false;
	}
	/*
	iRet = fprintf( pFile, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" );
	assert( iRet );
	*/

	//////////////////////////////////////////////////////////////////
	// save all data
	//////////////////////////////////////////////////////////////////
	VECTOR_DATAINFO_PTR::iterator	iDataInfo;
	sigma::uint32					dwIndent;
	VECTOR_TAG_REF					rVectorTagRef;
	CSulString						strLastNameUsed;

	dwIndent	= 0;
	iDataInfo = m_rVectorDataInfoPtr.begin();
	while ( iDataInfo!=m_rVectorDataInfoPtr.end() )
	{
		CSulXmlDataInfo* pDataInfo = (*iDataInfo).get();

		// check for end marker ( pAttr==0 is made by the elementEnd method )
		if ( pDataInfo->getAttr()==0 )
		{
			CSulString strNameTmp;

			// get the last known tag and remove it from our virtual stack
			strNameTmp = rVectorTagRef.begin()[rVectorTagRef.size()-1];
			rVectorTagRef.pop_back();

			// close at same tag?
			if ( strNameTmp==strLastNameUsed )
			{
				// create using the same start tag
				rFile->Print( "/>\n" );
				/*
				iRet = fprintf( pFile, "/>\n" );
				assert( iRet );
				*/
			}
			else
			{
				sigma::uint32 i;

				// indent
				for ( i=0; i<dwIndent-1; i++ )
				{
					rFile->Print( "\t" );
					/*
					iRet = fprintf( pFile, "\t" );
					assert( iRet );
					*/
				}

				// create a closing tag
				CSulString sTmp;
				sTmp.Format( "</%s>\n", strNameTmp );
				rFile->Print( sTmp );
				/*
				iRet = fprintf( pFile, "</%s>\n", strNameTmp.c_str() );
				assert( iRet );
				*/
			}

			strLastNameUsed = "";

			dwIndent--;
		}
		else
		{
			sigma::uint32 i;

			// if a tag is open,. close it
			if ( strLastNameUsed.size() )
			{
				rFile->Print( ">\n" );
				/*
				iRet = fprintf( pFile, ">\n" );
				assert( iRet );
				*/
			}

			strLastNameUsed = pDataInfo->getName();
			rVectorTagRef.push_back( pDataInfo->getName() );

			// indent
			for ( i=0; i<dwIndent; i++ )
			{
				rFile->Print( "\t" );
				/*
				iRet = fprintf( pFile, "\t" );
				assert( iRet );
				*/
			}

			// save tag
			CSulString sTmp;
			sTmp.Format( "<%s", pDataInfo->getName() );
			rFile->Print( sTmp );
			/*
			iRet = fprintf( pFile, "<%s", pDataInfo->GetName().c_str() );
			assert( iRet );
			*/

			// save attr
			sigma::uint32 dwCount = pDataInfo->getAttr()->getCount();

			CSulXmlAttr::VECTOR_ATTR_PTR::iterator iAttr;	

			iAttr = pDataInfo->getAttr()->m_rVectorAttrPtr.begin();
			while ( dwCount-- )
			{
				CSulString sTmp;
				sTmp.Format( " %s=\"%s\"", (*iAttr)->strName, (*iAttr)->strValue );
				/*
				iRet = fprintf( pFile, " %s=\"%s\"", (*iAttr)->strName.c_str(), (*iAttr)->strValue.c_str() );
				assert( iRet );
				*/

				++iAttr;
			}
	
			++dwIndent;
		}

		++iDataInfo;
	}

	// cleanup
	rFile->Close();
//	fclose( pFile );

	return true;
}

CSulXmlAttr* CSulXmlWriter::elementStart( const char* pszName )
{
	CSulXmlAttr* pAttr = new CSulXmlAttr;

	CSulXmlDataInfo* pDataInfo = new CSulXmlDataInfo;
	pDataInfo->setName( pszName );
	pDataInfo->setAttr( pAttr );
	m_rVectorDataInfoPtr.push_back( pDataInfo );

	return pAttr;
}

void CSulXmlWriter::elementEnd()
{
	CSulXmlDataInfo* pDataInfo = new CSulXmlDataInfo;
	m_rVectorDataInfoPtr.push_back( pDataInfo );
}


