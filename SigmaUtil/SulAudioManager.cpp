// SulAudioManager.cpp

#include "stdafx.h"
#include "SulAudioManager.h"
#include "alut.h"

void CSulAudioManager::init()
{
	ALCdevice* device;

	// Initialization 
	device = alcOpenDevice(NULL); // select the "preferred device" 

	// create and set current context
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
}

void CSulAudioManager::createBuffer( const CSulString& bufferName, const CSulString& file )
{
	ALuint buffer;
	alGenBuffers( 1, &buffer );

	ALsizei size,freq;
	ALenum  format;
	ALvoid  *data;
	alutLoadWAVFile( (ALbyte*)file.c_str(), &format, &data, &size, &freq, false );
	alBufferData( buffer, format, data, size, freq );
    alutUnloadWAV( format, data, size, freq );

	m_mapBuffers[bufferName] = buffer;
}

ALuint CSulAudioManager::getBuffer( const CSulString& bufferName )
{
	return m_mapBuffers[bufferName];
}