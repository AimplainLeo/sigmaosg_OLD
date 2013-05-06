// SulAudioManager.cpp

#include "stdafx.h"
#include "SulAudioManager.h"
#include "alut.h"
#include <iostream>

void CSulAudioManager::init()
{
	ALCdevice* device;

	// Initialization 
	device = alcOpenDevice(NULL); // select the "preferred device" 

	// create and set current context
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
}

bool CSulAudioManager::Errors()
{
	ALint      error;

	if ( (error=alGetError())!=AL_NO_ERROR )
	{
		std::cout << alGetString(error) << std::endl;
		return false;
	}

	return false;
}

bool CSulAudioManager::createBuffer( const CSulString& bufferName, const CSulString& file )
{
	ALuint buffer;
	alGenBuffers( 1, &buffer );
	if ( Errors() )
		return false;

	ALsizei size,freq;
	ALenum  format;
	ALvoid  *data;
	ALboolean loop;

	alutLoadWAVFile( (ALbyte*)file.c_str(), &format, &data, &size, &freq, &loop );
	if ( Errors() )
	{
		std::cout << "ERROR: alutLoadWAVFile failed to load sound: " << file << std::endl;
		return false;
	}

	alBufferData( buffer, format, data, size, freq );
	if ( Errors() )
		return false;

    alutUnloadWAV( format, data, size, freq );
	if ( Errors() )
		return false;

	m_mapBuffers[bufferName] = buffer;

	return true;
}

ALuint CSulAudioManager::getBuffer( const CSulString& bufferName )
{
	return m_mapBuffers[bufferName];
}