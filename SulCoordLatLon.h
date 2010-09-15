// SulCoordLatLon.h

#ifndef __SULCOORDLATLON_H__
#define __SULCOORDLATLON_H__

////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Latitude and Longitude can be defined in the following ways:
//	-DMS				: degrees, minutes, seconds (49°30'00"N, 123°30'00"W)
//	-DM					: degrees, minutes (49°30.0', -123°30.0'), (49d30.0m,-123d30.0')
//	-DD					: decimal degress (49.5000°,-123.5000°)
//
//	internal will this class keep all values as DD
//
////////////////////////////////////////////////////////////////////////////////////////////////

class CSulCoordLatLon : public osg::Referenced
{
public:
			CSulCoordLatLon( double lat, double lon );

	void	getLatitude( double& lat ) const;
	void	getLongitude( double& lon ) const;

private:
	double	m_lat;
	double	m_lon;
};

#endif // __SULCOORDLATLON_H__