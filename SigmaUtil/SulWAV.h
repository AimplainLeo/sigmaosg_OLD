// SulWAV.h

#ifndef __SULWAV_H__
#define __SULWAV_H__

#include "SulString.h"
#include "al.h"
#include "alc.h"

class CSulWav
{
private:
#pragma pack(push)
#pragma pack(1)
	/*
	* Struct to hold fmt subchunk data for WAVE files.
	*/
	struct WAVE_Format {
	  char subChunkID[4];
	  long subChunkSize;
	  short audioFormat;
	  short numChannels;
	  long sampleRate;
	  long byteRate;
	  short blockAlign;
	  short bitsPerSample;
	};

	/*
	 * Struct that holds the RIFF data of the Wave file.
	 * The RIFF data is the meta data information that holds,
	 * the ID, size and format of the wave file
	 */
	struct RIFF_Header {
	  char chunkID[4];
	  long chunkSize;//size not including chunkSize or chunkID
	  char format[4];
	};

	/*
	* Struct to hold the data of the wave file
	*/
	struct WAVE_Data {
	  char subChunkID[4]; //should contain the word data
	  long subChunk2Size; //Stores the size of the data block
	};
#pragma pack(pop)

public:
			CSulWav();
			CSulWav( const CSulString& filename );

	bool	load( const CSulString& filename );

	ALenum			getFormat();
	unsigned char*	getData();
	ALsizei			getSize();
	ALsizei			getFrequency();

private:
	ALenum			m_format;
	unsigned char*	m_data;
	ALsizei			m_size;
	ALsizei			m_freq;
};

#endif // __SULWAV_H__