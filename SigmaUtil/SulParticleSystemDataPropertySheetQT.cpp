// SulParticleSystemDataPropertySheetQT.cpp

#include "stdafx.h"
#include "SulParticleSystemDataPropertySheetQT.h"
#include "SulParticleSystemDataOsg.h"
#include "SulQtPropFloat.h"
#include "SulQtPropVec4.h"
#include "SulQtPropInt32.h"
#include "SulQtPropUint32.h"
#include "SulQtPropString.h"
#include "SulQtPropBool.h"
#include "SulQtPropVec3.h"
#include "SulQtPropCombobox.h"


CSulParticleSystemDataPropertySheetQt::CSulParticleSystemDataPropertySheetQt( CSulParticleSystemDataOsg* data )
{
	m_data = data;
	createPropertySheet();
}

CSulParticleSystemDataPropertySheetQt::~CSulParticleSystemDataPropertySheetQt()
{
}

void CSulParticleSystemDataPropertySheetQt::createPropertySheet()
{
	QVBoxLayout* layoutPropertySheets = new QVBoxLayout;
	setLayout( layoutPropertySheets );

	// general
	m_psGeneral = new CSulQtPropertySheet( "General" );
	layoutPropertySheets->addWidget( m_psGeneral );
	m_psGeneral->add( new CSulQtPropUint32( "Render Bin Num", m_data->m_renderBinNum ) );
	m_psGeneral->add( new CSulQtPropUint32( "Instance Count", m_data->m_instanceCount ) );
	
	// debris
	m_psDebris = new CSulQtPropertySheet( "Debris (Experimintal)" );
	layoutPropertySheets->addWidget( m_psDebris );
	m_psDebris->add( new CSulQtPropBool( "Enabled", m_data->m_debrisEnabled ) );
	m_psDebris->add( new CSulQtPropFloat( "Life Time", m_data->m_debrisLifeTime ) );
	m_psDebris->add( new CSulQtPropBool( "Endless", m_data->m_debrisEndless ) );
	m_psDebris->add( new CSulQtPropFloat( "Position Offset Min", m_data->m_debrisPosOffsetMin ) );
	m_psDebris->add( new CSulQtPropFloat( "Position Offset Max", m_data->m_debrisPosOffsetMax ) );
	m_psDebris->add( new CSulQtPropFloat( "Speed Min", m_data->m_debrisSpeedMin ) );
	m_psDebris->add( new CSulQtPropFloat( "Speed Max", m_data->m_debrisSpeedMax ) );
	m_psDebris->add( new CSulQtPropFloat( "Theta Min", m_data->m_debrisThetaMin ) );
	m_psDebris->add( new CSulQtPropFloat( "Theta Max", m_data->m_debrisThetaMax ) );
	m_psDebris->add( new CSulQtPropFloat( "Phi Min", m_data->m_debrisPhiMin ) );
	m_psDebris->add( new CSulQtPropFloat( "Phi Max", m_data->m_debrisPhiMax ) );

	// particle
	m_psParticles = new CSulQtPropertySheet( "Particle" );
	layoutPropertySheets->addWidget( m_psParticles );
	m_psParticles->add( new CSulQtPropFloat( "Life Time (sec)", m_data->m_particleLifeTime ) );
	m_psParticles->add( new CSulQtPropFloat( "Size Min", m_data->m_particleSizeMin ) );
	m_psParticles->add( new CSulQtPropFloat( "Size Max", m_data->m_particleSizeMax ) );
	m_psParticles->add( new CSulQtPropFloat( "Alpha Min", m_data->m_particleAlphaMin ) );
	m_psParticles->add( new CSulQtPropFloat( "Alpha Max", m_data->m_particleAlphaMax ) );
	m_psParticles->add( new CSulQtPropCombobox( "AlphaInterplator", m_data->m_particleAlphaInterpolator, "LINEAR,QUADRATIC" ) );
	m_psParticles->add( new CSulQtPropVec4( "Color Min", m_data->m_particleColorMin ) );
	m_psParticles->add( new CSulQtPropVec4( "Color Max", m_data->m_particleColorMax ) );
	m_psParticles->add( new CSulQtPropFloat( "Radius", m_data->m_particleRadius ) );
	m_psParticles->add( new CSulQtPropFloat( "Mass", m_data->m_particleMass ) );
	m_psParticles->add( new CSulQtPropInt32( "Texture Tile S", m_data->m_particleTextureTileS ) );
	m_psParticles->add( new CSulQtPropInt32( "Texture Tile T", m_data->m_particleTextureTileT ) );
	m_psParticles->add( new CSulQtPropUint32( "Texture Tile Start", m_data->m_particleTextureTileStart ) );
	m_psParticles->add( new CSulQtPropUint32( "Texture Tile End", m_data->m_particleTextureTileEnd ) );

	// particle system
	m_psParticleSystem = new CSulQtPropertySheet( "Particle System" );
	layoutPropertySheets->addWidget( m_psParticleSystem );
	m_psParticleSystem->add( new CSulQtPropCombobox( "Sort Mode", m_data->m_psSortMode, "NO_SORT,SORT_FRONT_TO_BACK,SORT_BACK_TO_FRONT" ) );
	m_psParticleSystem->add( new CSulQtPropString( "Texture File", m_data->m_psTextureFile ) );
	m_psParticleSystem->add( new CSulQtPropBool( "Emissive", m_data->m_psUseEmissive ) );
	m_psParticleSystem->add( new CSulQtPropBool( "Lighting", m_data->m_psUseLighting ) );

	// modular emitter
	m_psModularEmitter = new CSulQtPropertySheet( "Emitter: Modular" );
	layoutPropertySheets->addWidget( m_psModularEmitter );
	m_psModularEmitter->add( new CSulQtPropFloat( "Compensation Ratio", m_data->m_emitterCompensationRatio ) );
	m_psModularEmitter->add( new CSulQtPropBool( "Endless", m_data->m_emitterEndless ) );
	m_psModularEmitter->add( new CSulQtPropFloat( "Start Time", m_data->m_emitterStartTime ) );
	m_psModularEmitter->add( new CSulQtPropFloat( "Life Time", m_data->m_emitterLifeTime ) );

	// Random Rate Counter
	m_psCounterRandomRate = new CSulQtPropertySheet( "Counter: Random Rate" );
	layoutPropertySheets->addWidget( m_psCounterRandomRate );
	m_psCounterRandomRate->add( new CSulQtPropFloat( "Random Rate Min", m_data->m_counterRandomRateMin ) );
	m_psCounterRandomRate->add( new CSulQtPropFloat( "Random Rate Max", m_data->m_counterRandomRateMax ) );

	// Sector Placer
	m_psSectorPlacer = new CSulQtPropertySheet( "Sector Placer" );
	layoutPropertySheets->addWidget( m_psSectorPlacer );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Min", m_data->m_sectorRadiusMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Radius Max", m_data->m_sectorRadiusMax ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Phi Min", m_data->m_sectorPhiMin ) );
	m_psSectorPlacer->add( new CSulQtPropFloat( "Phi Max", m_data->m_sectorPhiMax ) );

	// Radial Shooter
	m_psShooterRadial = new CSulQtPropertySheet( "Shooter: Radial" );
	layoutPropertySheets->addWidget( m_psShooterRadial );
	m_psShooterRadial->add( new CSulQtPropFloat( "Theta Min", m_data->m_shooterRadialThetaMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Theta Max", m_data->m_shooterRadialThetaMax ) );
	m_psShooterRadial->add( new CSulQtPropBool( "Theta Random Lock", m_data->m_shooterRadialThetaRandomLock ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Phi Min", m_data->m_shooterRadialPhiMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Phi Max", m_data->m_shooterRadialPhiMax ) );
	m_psShooterRadial->add( new CSulQtPropBool( "Phi Random Lock", m_data->m_shooterRadialPhiRandomLock ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Initial Speed Min", m_data->m_shooterRadialInitialSpeedMin ) );
	m_psShooterRadial->add( new CSulQtPropFloat( "Initial Speed Max", m_data->m_shooterRadialInitialSpeedMax ) );
	m_psShooterRadial->add( new CSulQtPropBool( "Initial Speed Random Lock", m_data->m_shooterRadialInitialSpeedRandomLock ) );
	m_psShooterRadial->add( new CSulQtPropVec3( "Initial Rotation Min", m_data->m_shooterRadialInitialRotationMin ) );
	m_psShooterRadial->add( new CSulQtPropVec3( "Initial Rotation Max", m_data->m_shooterRadialInitialRotationMax ) );
	m_psShooterRadial->add( new CSulQtPropBool( "Initial Rotation Random Lock", m_data->m_shooterRadialInitialRotationRandomLock ) );

	// Program Fluid
	m_psProgramFluid = new CSulQtPropertySheet( "Program: Fluid" );
	layoutPropertySheets->addWidget( m_psProgramFluid );
	m_psProgramFluid->add( new CSulQtPropFloat( "Density kg/m3",m_data-> m_programFluidDensity ) );
	m_psProgramFluid->add( new CSulQtPropFloat( "Viscosity",m_data-> m_programFluidViscosity ) );
	m_psProgramFluid->add( new CSulQtPropVec3( "Wind", m_data->m_programFluidWind ) );
}

void CSulParticleSystemDataPropertySheetQt::updateFromUI()
{
	m_psGeneral->updateFromUI();
	m_psDebris->updateFromUI();
	m_psParticles->updateFromUI();
	m_psParticleSystem->updateFromUI();
	m_psModularEmitter->updateFromUI();
	m_psCounterRandomRate->updateFromUI();
	m_psSectorPlacer->updateFromUI();
	m_psShooterRadial->updateFromUI();
	m_psProgramFluid->updateFromUI();
}

void CSulParticleSystemDataPropertySheetQt::updateToUI()
{
	m_psGeneral->updateToUI();
	m_psDebris->updateToUI();
	m_psParticles->updateToUI();
	m_psParticleSystem->updateToUI();
	m_psModularEmitter->updateToUI();
	m_psCounterRandomRate->updateToUI();
	m_psSectorPlacer->updateToUI();
	m_psShooterRadial->updateToUI();
	m_psProgramFluid->updateToUI();
}

