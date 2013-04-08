// SulParticleSystemLoadInstancer.cpp

#include "stdafx.h"
#include "SulParticleSystemLoadInstancer.h"
#include "SulXmlReader.h"
#include "SulParticleSystemDataOsg.h"

void CSulParticleSystemLoadInstancer::create( const osg::Vec3& pos )
{

}

bool CSulParticleSystemLoadInstancer::load( const CSulString& file )
{
	CSulXmlReader reader;
	if ( !reader.load( file ) )
		return false;

	CSulXmlReader::VEC_XMLNODE vecXmlNode;
	reader.findTags( "ParticleSystemDataOsg", vecXmlNode );

	CSulXmlReader::VEC_XMLNODE::iterator i = vecXmlNode.begin();
	CSulXmlReader::VEC_XMLNODE::iterator ie = vecXmlNode.end();
	while ( i!=ie )
	{
		CSulXmlNodeTag* tagParticleSystemDataOsg = (*i);

		CSulString title = tagParticleSystemDataOsg->getAttrAsString( "Title", "" );
		bool bEnabled = tagParticleSystemDataOsg->getAttrAsBool( "Enabled", true );

		CSulParticleSystemDataOsg* data = new CSulParticleSystemDataOsg( title );
		data->setDefaultValues();
		data->load( tagParticleSystemDataOsg );
		
		++i;
	}

	return true;
}