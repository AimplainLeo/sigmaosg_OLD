// SulCoordLatLon.cpp

#include "stdafx.h"
#include "SulCoordLatLon.h"

CSulCoordLatLon::CSulCoordLatLon( double lat, double lon ) :
m_lat(lat),
m_lon(lon)
{
}

void CSulCoordLatLon::getLatitude( double& lat ) const
{
	lat = m_lat;
}

void CSulCoordLatLon::getLongitude( double& lon ) const
{
	lon = m_lon;
}

