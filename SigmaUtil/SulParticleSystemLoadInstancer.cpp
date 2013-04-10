// SulParticleSystemLoadInstancer.cpp

#include "stdafx.h"
#include "SulParticleSystemLoadInstancer.h"
#include "SulXmlReader.h"

CSulParticleSystemLoadInstancer::CSulParticleSystemLoadInstancer() :
osg::Group()
{
	m_wind.set( 0,0,0 );
	m_particleSystem = new CSulParticleSystemOsg;
	addChild( m_particleSystem );
}

void CSulParticleSystemLoadInstancer::create( const osg::Vec3& pos )
{
	VEC_DATA::iterator i = m_vecData.begin();
	VEC_DATA::iterator ie = m_vecData.end();
	while ( i!=ie )
	{
		// create particle system
		CSulParticleSystemContainerOsg* psContainer = new CSulParticleSystemContainerOsg( (*i), this );

		// place it
		m_particleSystem->create( psContainer, pos );

		++i;
	}

	m_particleSystem->setWind( m_wind );
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
		
		m_vecData.push_back( data );

		++i;
	}

	return true;
}

void CSulParticleSystemLoadInstancer::setWind( const osg::Vec3& wind )
{
	m_wind = wind;
	m_particleSystem->setWind( m_wind );
}