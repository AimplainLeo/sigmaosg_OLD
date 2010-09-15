// SulCoordUTM.cpp

#include "stdafx.h"
#include "SulCoordUTM.h"

const double MAX_DELTA_LONG  = ((osg::PI * 90)/180.0);    /* 90 degrees in radians */

/**
* The reciprocal of the flattening parameter (WGS 84).
*/
const double flatteningReciprocal = 298.257223563;

/**
* The length of the semi-major axis, in meters (WGS 84).
*/
const double semiMajorAxis = 6378137.0;

/* Ellipsoid parameters in WGS 84 */
const double Geocent_a = semiMajorAxis;  /* Semi-major axis of ellipsoid in meters */
const double Geocent_f = 1 / flatteningReciprocal; /* Flattening of ellipsoid           */

///Scale used in UTM calculations.
const double CentralMeridianScale = 0.9996;


CSulCoordUTM::CSulCoordUTM( const CSulCoordLatLon& latlon )
{
	calcZone( latlon, m_zoneEW, m_zoneNS );
	calcOffset( latlon, m_zoneEW, m_zoneNS, m_easting, m_northing );
}

CSulCoordUTM::CSulCoordUTM( const CSulCoordGeodetic& coord )
{
	calcZone( coord.getLatLon(), m_zoneEW, m_zoneNS );
	calcOffset( coord.getLatLon(), m_zoneEW, m_zoneNS, m_easting, m_northing );
}

void CSulCoordUTM::calcOffset( const CSulCoordLatLon& latlon, sigma::uint32 zoneEW, sigma::uint8 zoneNS, double& easting, double& northing )
{
	double Origin_Latitude = 0.0;
	double Central_Meridian = 0.0;
	double False_Easting = 500000;
	double False_Northing = 0;
	
	double lat;
	double lon;

	latlon.getLatitude( lat );
	latlon.getLongitude( lon );
	
	lat = osg::DegreesToRadians( lat );
	lon = osg::DegreesToRadians( lon );

	if ( lon<0 )
        lon += (2*osg::PI) + 1.0e-10;

	if ( zoneEW >= 31)
		Central_Meridian = osg::DegreesToRadians(double(6 * zoneEW - 183));
	else
		Central_Meridian = osg::DegreesToRadians(double(6 * zoneEW + 177));

	// If we are projecting in the southern hemisphere, set the false northing.
	if ( zoneNS=='S' || zoneNS=='s' )
	{
		 False_Northing = 10000000;
	}

	CSulTranverseMercatorParameters params;
	
	params.calcTransverseMercatorParameters(
		Geocent_a, 
		Geocent_f, 
		Origin_Latitude,
		Central_Meridian, 
		False_Easting, 
		False_Northing, 
		CentralMeridianScale);
	
	params.convertGeodeticToTransverseMercator( params, lat, lon, easting, northing );
}


void CSulCoordUTM::calcZone( const CSulCoordLatLon& latlon, sigma::uint32& ewZone, sigma::uint8& nsZone )
{
	double lat;
	double lon;

	latlon.getLatitude( lat );
	latlon.getLongitude( lon );

	osg::clampBetween( lat, -80.0, 84.0 );

	long Lat_Degrees = long(lat);
	long Long_Degrees = long(lon);

	unsigned long zone;

	if (lon < 180.0)
	zone = long(31 + (lon / 6.0));
	else
	zone = long((lon/ 6.0) - 29);

	if (zone > 60)
	zone = 1;

	/* UTM special cases */
	if ((Lat_Degrees > 55) && (Lat_Degrees < 64) && (Long_Degrees > -1)
	&& (Long_Degrees < 3))
	zone = 31;
	else if ((Lat_Degrees > 55) && (Lat_Degrees < 64) && (Long_Degrees > 2)
	&& (Long_Degrees < 12))
	zone = 32;
	else if ((Lat_Degrees > 71) && (Long_Degrees > -1) && (Long_Degrees < 9))
	zone = 31;
	else if ((Lat_Degrees > 71) && (Long_Degrees > 8) && (Long_Degrees < 21))
	zone = 33;
	else if ((Lat_Degrees > 71) && (Long_Degrees > 20) && (Long_Degrees < 33))
	zone = 35;
	else if ((Lat_Degrees > 71) && (Long_Degrees > 32) && (Long_Degrees < 42))
	zone = 37;

	ewZone = zone;


	/* Calculate UTM zone letter (north-south direction) */
	if (lat < 80.0)
	{
	nsZone = 'C' + (char)floor((lat + 80.0) / 8.0);
	//Skip the letters I and O. They aren't used.
	if (nsZone >= 'I')
	nsZone ++;
	if (nsZone >= 'O')
	nsZone ++;
	}
	else /* 80 to 84 are also grid 'X' */
	nsZone = 'X';
}

osg::Vec3d CSulCoordUTM::getXYZ( const CSulCoordLatLon& latlon )
{
	double easting;
	double northing;

	calcOffset( latlon, m_zoneEW, m_zoneNS, easting, northing );

	osg::Vec3d v(
		easting-m_easting,
		northing-m_northing,
		0.0
	);

	return v;
}