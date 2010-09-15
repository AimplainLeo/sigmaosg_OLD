// SulCoordUTM.h

#ifndef __SULCOORDUTM_H__
#define __SULCOORDUTM_H__

#include "SulTypes.h"
#include "SulCoordGeodetic.h"
#include "SulCoordLatLon.h"
#include "SulTranverseMercatorParameters.h"

////////////////////////////////////////////////////////////////////////////////////////////////
//
//	UTM : Universal Transverse Mercator 
//
//	UTM defines a flat 2D cartesian coordinate system
//	there are 60 zones
//	zone 1 is bounded by longitude 180 to 174W
//	UTM system divides the surface of Earth between 80S and 84N into 60 zones
//
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CSulCoordUTM : public osg::Referenced
{
public:
				CSulCoordUTM( const CSulCoordLatLon& latlon );
				CSulCoordUTM( const CSulCoordGeodetic& coord );

	osg::Vec3d	getXYZ( const CSulCoordLatLon& latlon );

private:
	void		calcZone( const CSulCoordLatLon& latlon, sigma::uint32& ewZone, sigma::uint8& nsZone );
	void		calcOffset( const CSulCoordLatLon& latlon, sigma::uint32 zoneEW, sigma::uint8 zoneNS, double& easting, double& northing );

private:
	sigma::uint32	m_zoneEW;			// 1-60
	sigma::uint8	m_zoneNS;			// A-X

	double			m_easting;
	double			m_northing;
};

#endif // __SULCOORDUTM_H__