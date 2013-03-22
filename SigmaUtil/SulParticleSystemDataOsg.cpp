// SulParticleSystemDataOsg.cpp

#include "stdafx.h"
#include "SulParticleSystemDataOsg.h"
#include "SulXmlWriter.h"
#include "SulXmlReader.h"

void CSulParticleSystemDataOsg::setDefaultValues()
{
	// osgParticle::Particle parameter values
	m_particleLifeTime					= 3.0f;
	m_particleSizeMin					= 0.75f;
	m_particleSizeMax					= 3.0f;
	m_particleAlphaMin					= 1.0f;
	m_particleAlphaMax					= 0.0f;
	m_particleColorMin					= osg::Vec4(1,1,1,1);
	m_particleColorMax					= osg::Vec4(1,1,1,1);
	m_particleRadius					= 0.5f;
	m_particleMass						= 0.6f;
	m_particleTextureTileS				= 4;
	m_particleTextureTileT				= 4;
	m_particleTextureTileStart			= 0;
	m_particleTextureTileEnd			= 15;

	// osgParticle::ParticleSystem parameter values
	m_psTextureFile						= "images/animated_smoke.png";
	m_psUseEmissive						= false;
	m_psUseLighting						= false;

	// osgParticle::ModularEmitter parameter values
	m_emitterCompensationRatio			= 1.5f;
	m_emitterEndless					= false;
	m_emitterStartTime					= 0.0f;
	m_emitterLifeTime					= 10.0f;

	// osgParticle::RandomRateCounter parameter values
	m_counterRandomRateMin				= 10.0f;
	m_counterRandomRateMax				= 10.0f;

	// osgParticle::SectorPlacer parameter values
	m_sectorRadiusMin					= 0.0f;
	m_sectorRadiusMax					= 0.1f;
	m_sectorPhiMin						= 0.0f;
	m_sectorPhiMax						= 2*osg::PI;

	// osgParticle::RadialShooter parameter values
	m_shooterRadialThetaMin				= 0.0f;
	m_shooterRadialThetaMax				= 0.4f;
	m_shooterRadialPhiMin				= 0.0f;
	m_shooterRadialPhiMax				= 2*osg::PI;
	m_shooterRadialInitialSpeedMin		= 0.0f;
	m_shooterRadialInitialSpeedMax		= 20.0f;
	m_shooterRadialInitialRotationMin	= osg::Vec3(0,0,-1);
	m_shooterRadialInitialRotationMax	= osg::Vec3(0,0,1);

	// osgParticle::FluidProgram parameter values
	m_programFluidDensity				= 1.2041f;
	m_programFluidWind					= osg::Vec3(0,0,0);
}

bool CSulParticleSystemDataOsg::save( const CSulString& file )
{
	CSulXmlWriter	writer( "ParticleSystemDataOsg" );

	CSulXmlAttr* pAttr;

	// particle
	pAttr = writer.elementStart( "Particle" );
	pAttr->add( "LifeTime", m_particleLifeTime );
	pAttr->add( "SizeMin", m_particleSizeMin );
	pAttr->add( "SizeMax", m_particleSizeMax );
	pAttr->add( "AlphaMin", m_particleAlphaMin );
	pAttr->add( "AlphaMax", m_particleAlphaMax );
	pAttr->add( "ColorMin", m_particleColorMin );
	pAttr->add( "ColorMax", m_particleColorMax );
	pAttr->add( "Radius", m_particleRadius );
	pAttr->add( "Mass", m_particleMass );
	pAttr->add( "TextureTileS", m_particleTextureTileS );
	pAttr->add( "TextureTileT", m_particleTextureTileT );
	pAttr->add( "TextureTileStart", m_particleTextureTileStart );
	pAttr->add( "TextureTileEnd", m_particleTextureTileEnd );
	writer.elementEnd();

	// particle system
	pAttr = writer.elementStart( "ParticleSystem" );
	pAttr->add( "TextureFile", m_psTextureFile );
	pAttr->add( "UseEmissive", m_psUseEmissive );
	pAttr->add( "UseLighting", m_psUseLighting );
	pAttr->add( "SortMode", m_psSortMode );
	writer.elementEnd();

	// emitter: Modular Emitter
	pAttr = writer.elementStart( "ModularEmitter" );
	pAttr->add( "CompensationRatio", m_emitterCompensationRatio );
	pAttr->add( "EndLess", m_emitterEndless );
	pAttr->add( "StartTime", m_emitterStartTime );
	pAttr->add( "LifeTime", m_emitterLifeTime );
	writer.elementEnd();

	// emitter: Random Rate Counter
	pAttr = writer.elementStart( "RandomRateCounter" );
	pAttr->add( "RateMin", m_counterRandomRateMin );
	pAttr->add( "RateMax", m_counterRandomRateMax );
	writer.elementEnd();

	// Sector Placer
	pAttr = writer.elementStart( "SectorPlacer" );
	pAttr->add( "RadiusMin", m_sectorRadiusMin );
	pAttr->add( "RadiusMax", m_sectorRadiusMax );
	pAttr->add( "PhiMin", m_sectorPhiMin );
	pAttr->add( "PhiMax", m_sectorPhiMax );
	writer.elementEnd();

	// Radial Shooter
	pAttr = writer.elementStart( "RadialShooter" );
	pAttr->add( "ThetaMin", m_shooterRadialThetaMin );
	pAttr->add( "ThetaMax", m_shooterRadialThetaMax );
	pAttr->add( "PhiMin", m_shooterRadialPhiMin );
	pAttr->add( "PhiMax", m_shooterRadialPhiMax );
	pAttr->add( "InitialSpeedMin", m_shooterRadialInitialSpeedMin );
	pAttr->add( "InitialSpeedMax", m_shooterRadialInitialSpeedMax );
	pAttr->add( "InitialRotationMin", m_shooterRadialInitialRotationMin );
	pAttr->add( "InitialRotationMax", m_shooterRadialInitialRotationMax );
	writer.elementEnd();

	// Fluid Program
	pAttr = writer.elementStart( "FluidProgram" );
	pAttr->add( "Density", m_programFluidDensity );
	pAttr->add( "Wind", m_programFluidWind );
	writer.elementEnd();

	if ( !writer.Save( file.c_str() ) )
		return false;

	return true;
}

bool CSulParticleSystemDataOsg::load( const CSulString& file )
{
	CSulXmlReader reader;
	if ( !reader.load( file ) )
		return false;

	CSulXmlNodeTag* tag;
		
	tag = reader.findTag( "Particle" );
	m_particleLifeTime					= tag->getAttrAsFloat( "LifeTime", 2.5f );
	m_particleSizeMin					= tag->getAttrAsFloat( "SizeMin", 2.5f );
	m_particleSizeMax					= tag->getAttrAsFloat( "SizeMax", 2.5f );
	m_particleAlphaMin					= tag->getAttrAsFloat( "AlphaMin", 2.5f );
	m_particleAlphaMax					= tag->getAttrAsFloat( "AlphaMax", 2.5f );
	m_particleColorMin					= tag->getAttrAsVec4( "ColorMin", osg::Vec4(0,0,0,0) );
	m_particleColorMax					= tag->getAttrAsVec4( "ColorMax", osg::Vec4(0,0,0,0) );
	m_particleRadius					= tag->getAttrAsFloat( "Radius", 2.5f );
	m_particleMass						= tag->getAttrAsFloat( "Mass", 2.5f );
	m_particleTextureTileS				= tag->getAttrAsFloat( "TextureTileS", 2.5f );
	m_particleTextureTileT				= tag->getAttrAsFloat( "TextureTileT", 2.5f );
	m_particleTextureTileStart			= tag->getAttrAsFloat( "TextureTileStart", 2.5f );
	m_particleTextureTileEnd			= tag->getAttrAsFloat( "TextureTileEnd", 2.5f );

	tag = reader.findTag( "ParticleSystem" );
	m_psTextureFile						= tag->getAttrAsString( "TextureFile", "" );
	m_psUseEmissive						= tag->getAttrAsBool( "UseEmissive", false );
	m_psUseLighting						= tag->getAttrAsBool( "UseLighting", false );
	m_psSortMode						= tag->getAttrAsString( "SortMode", "NO_SORT" );

	tag = reader.findTag( "ModularEmitter" );
	m_emitterCompensationRatio			= tag->getAttrAsFloat( "CompensationRatio", 1.5f );
	m_emitterEndless					= tag->getAttrAsBool( "EndLess", false );
	m_emitterStartTime					= tag->getAttrAsFloat( "StartTime", 0.0f );
	m_emitterLifeTime					= tag->getAttrAsFloat( "LifeTime", 3.0f );

	tag = reader.findTag( "RandomRateCounter" );
	m_counterRandomRateMin				= tag->getAttrAsFloat( "RateMin", 10.0f );
	m_counterRandomRateMax				= tag->getAttrAsFloat( "RateMax", 10.0f );

	tag = reader.findTag( "SectorPlacer" );
	m_sectorRadiusMin					= tag->getAttrAsFloat( "RadiusMin", 0.0f );
	m_sectorRadiusMax					= tag->getAttrAsFloat( "RadiusMax", 0.1f );
	m_sectorPhiMin						= tag->getAttrAsFloat( "PhiMin", 0.0f );
	m_sectorPhiMax						= tag->getAttrAsFloat( "PhiMax", 0.0f );

	tag = reader.findTag( "RadialShooter" );
	m_shooterRadialThetaMin				= tag->getAttrAsFloat( "ThetaMin", 0.0f );
	m_shooterRadialThetaMax				= tag->getAttrAsFloat( "ThetaMax", 0.0f );
	m_shooterRadialPhiMin				= tag->getAttrAsFloat( "PhiMin", 0.0f );
	m_shooterRadialPhiMax				= tag->getAttrAsFloat( "PhiMax", 0.0f );
	m_shooterRadialInitialSpeedMin		= tag->getAttrAsFloat( "InitialSpeedMin", 0.0f );
	m_shooterRadialInitialSpeedMax		= tag->getAttrAsFloat( "InitialSpeedMax", 0.0f );
	m_shooterRadialInitialRotationMin	= tag->getAttrAsVec3( "InitialRotationMin", osg::Vec3(0,0,-1) );
	m_shooterRadialInitialRotationMax	= tag->getAttrAsVec3( "InitialRotationMax", osg::Vec3(0,0,1) );

	tag = reader.findTag( "FluidProgram" );
	m_programFluidDensity				= tag->getAttrAsFloat( "Density", 0.0f );
	m_programFluidWind					= tag->getAttrAsVec3( "Wind", osg::Vec3(0,0,0) );

	return true;
}
