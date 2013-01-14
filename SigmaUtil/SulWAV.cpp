// SulWAV.h

#include "stdafx.h"
#include "SulWAV.h"
#include "SulFile.h"
#include <osgDB/FileUtils>
#include <iostream>

CSulWav::CSulWav()
{
}

CSulWav::CSulWav( const CSulString& filename )
{
	load( filename );
}

bool CSulWav::load( const CSulString& filename )
{
	osg::ref_ptr<CSulFile> file = new CSulFile;

	if ( !file->Load( filename ) )
		return false;

	sigma::uint8* data = file->GetDataPtr();

	sigma::uint32 sizeRiffHeader = sizeof(RIFF_Header);
	sigma::uint32 sizeWaveFormat = sizeof(WAVE_Format);

	RIFF_Header* riffHeader = (RIFF_Header*)&data[0];
	WAVE_Format* waveFormat = (WAVE_Format*)&data[sizeof(RIFF_Header)];

	sigma::uint32 ep = waveFormat->subChunkSize>16 ? waveFormat->subChunkSize-16:0;

	short t = (short)(data[34]);

	// from this point on only chunks (remember wav file can have optional chunks in it before data)
	// we read until we have read the data chunk
	sigma::int32 i = sizeof(RIFF_Header)+sizeof(WAVE_Format)+ep;
	while ( 1 )
	{
		CSulString chunkId = "";
		chunkId += data[i+0];
		chunkId += data[i+1];
		chunkId += data[i+2];
		chunkId += data[i+3];

		if ( chunkId=="fact" )
			i+= 12;
		else if (chunkId=="data" )
		{
			WAVE_Data* waveData = (WAVE_Data*)&data[i];
			i+= sizeof(WAVE_Data);
			m_data = new unsigned char[waveData->subChunk2Size];
			memcpy( m_data, &data[i], waveData->subChunk2Size );

			// convience methods for openAL
			m_size = waveData->subChunk2Size;
			m_freq = waveFormat->sampleRate;

			if (waveFormat->numChannels == 1) 
			{
				if (waveFormat->bitsPerSample == 8 )
					m_format = AL_FORMAT_MONO8;
				else if (waveFormat->bitsPerSample == 16)
				m_format = AL_FORMAT_MONO16;
			} 
			else if (waveFormat->numChannels == 2) 
			{
				if (waveFormat->bitsPerSample == 8 )
					m_format = AL_FORMAT_STEREO8;
				else if (waveFormat->bitsPerSample == 16)
				m_format = AL_FORMAT_STEREO16;
			}
			break;
		}
		else
		{
			// unknown chunk
			assert( 0 );
		}
	}
	
	return true;
}

#if 0
bool CSulWav::load( const CSulString& filename ) 
{
  //Local Declarations
  FILE* soundFile = NULL;
  WAVE_Format wave_format;
  RIFF_Header riff_header;
  WAVE_Data wave_data;
 
  try {
	CSulString fileFound = osgDB::findDataFile( filename );

    soundFile = fopen(fileFound.c_str(), "rb");
    if (!soundFile)
      throw (filename);
 
    // Read in the first chunk into the struct
    fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);
 
    //check for RIFF and WAVE tag in memeory
    if ((riff_header.chunkID[0] != 'R' ||
         riff_header.chunkID[1] != 'I' ||
         riff_header.chunkID[2] != 'F' ||
         riff_header.chunkID[3] != 'F') &&
        (riff_header.format[0] != 'W' ||
         riff_header.format[1] != 'A' ||
         riff_header.format[2] != 'V' ||
         riff_header.format[3] != 'E'))
             throw ("Invalid RIFF or WAVE Header");
 
    //Read in the 2nd chunk for the wave info
    fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
    //check for fmt tag in memory
    if (wave_format.subChunkID[0] != 'f' ||
        wave_format.subChunkID[1] != 'm' ||
        wave_format.subChunkID[2] != 't' ||
        wave_format.subChunkID[3] != ' ')
             throw ("Invalid Wave Format");
 
    //check for extra parameters;
    if (wave_format.subChunkSize > 16)
	{
		fseek(soundFile, wave_format.subChunkSize-16, SEEK_CUR);
		int a =1;
	}
 
    //Read in the the last byte of data before the sound file
    fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
    //check for data tag in memory
    if (wave_data.subChunkID[0] != 'd' ||
        wave_data.subChunkID[1] != 'a' ||
        wave_data.subChunkID[2] != 't' ||
        wave_data.subChunkID[3] != 'a')
             throw ("Invalid data header");
 
    //Allocate memory for data
    m_data = new unsigned char[wave_data.subChunk2Size];
 
    // Read in the sound data into the soundData variable
    if (!fread(m_data, wave_data.subChunk2Size, 1, soundFile))
        throw ("error loading WAVE data into struct!");
 
    //Now we set the variables that we passed in with the
    //data from the structs
    m_size = wave_data.subChunk2Size;
    m_freq = wave_format.sampleRate;
    //The format is worked out by looking at the number of
    //channels and the bits per sample.
    if (wave_format.numChannels == 1) {
        if (wave_format.bitsPerSample == 8 )
            m_format = AL_FORMAT_MONO8;
        else if (wave_format.bitsPerSample == 16)
            m_format = AL_FORMAT_MONO16;
    } else if (wave_format.numChannels == 2) {
        if (wave_format.bitsPerSample == 8 )
            m_format = AL_FORMAT_STEREO8;
        else if (wave_format.bitsPerSample == 16)
            m_format = AL_FORMAT_STEREO16;
    }
	/*
    //create our openAL buffer and check for success
    alGenBuffers(1, buffer);
    if ((alGetError()) != AL_NO_ERROR) 
		throw( "error SulWav alGenBuffers" ); 

    //now we put our data into the openAL buffer and
    //check for success
    alBufferData(*buffer, *format, (void*)data,
                 *size, *frequency);
    if ((alGetError()) != AL_NO_ERROR) 
		throw( "error SulWav alBufferData" ); 

    //clean up and return true if successful
    fclose(soundFile);
	*/
    return true;
  } catch(std::string error) {
    //our catch statement for if we throw a string
    std::cerr << error << " : trying to load "
              << filename << std::endl;
    //clean up memory if wave loading fails
    if (soundFile != NULL)
        fclose(soundFile);
    //return false to indicate the failure to load wave
    return false;
  }
}
#endif

ALenum CSulWav::getFormat()
{
	return m_format;
}

unsigned char* CSulWav::getData()
{
	return m_data;
}

ALsizei CSulWav::getSize()
{
	return m_size;
}

ALsizei CSulWav::getFrequency()
{
	return m_freq;
}