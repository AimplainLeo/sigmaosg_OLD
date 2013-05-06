// SulAudioManager.h

#ifndef __SULAUDIOMANAGER_H__
#define __SULAUDIOMANAGER_H__

#include "SulString.h"
#include <al.h>

class CSulAudioManager
{
public:
	void init();

	bool createBuffer( const CSulString& bufferName, const CSulString& file );

	ALuint getBuffer( const CSulString& bufferName );

private:
	bool Errors();

private:
	typedef std::map< CSulString, ALuint >	MAP_BUFFERS;
	MAP_BUFFERS								m_mapBuffers;
};

#endif // __SULAUDIOMANAGER_H__