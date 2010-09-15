// SulTranverseMercatorParameters.h

#ifndef __SULTRANVERSEMERCATORPARAMETERS_H__
#define __SULTRANVERSEMERCATORPARAMETERS_H__

class CSulTranverseMercatorParameters
{
public:
	CSulTranverseMercatorParameters();

	void calcTransverseMercatorParameters(
		double a, 
		double f, 
		double Origin_Latitude,
		double Central_Meridian, 
		double False_Easting,
		double False_Northing, 
		double Scale_Factor );

	void convertGeodeticToTransverseMercator(
		const CSulTranverseMercatorParameters& params,
		double Latitude,
		double Longitude,
		double& Easting,
		double& Northing );

      double SPHTMD(double Latitude) const;
      double SPHSN(double Latitude) const;
      double DENOM(double Latitude) const;
      double SPHSR(double Latitude) const;

public:
	double TranMerc_a;         /* Semi-major axis of ellipsoid i meters */
	double TranMerc_es; /* Eccentricity (0.08181919084262188000) squared */

	/* Isometeric to geodetic latitude parameters, default to WGS 84 */
	double TranMerc_ap;
	double TranMerc_bp;
	double TranMerc_cp;
	double TranMerc_dp;
	double TranMerc_ep;

	/* Transverse_Mercator projection Parameters */
	double TranMerc_Origin_Lat;           /* Latitude of origin in radians */
	double TranMerc_Origin_Long;          /* Longitude of origin in radians */
	double TranMerc_False_Northing;       /* False northing in meters */
	double TranMerc_False_Easting;        /* False easting in meters */
	double TranMerc_Scale_Factor;         /* Scale factor  */

	/* Maximum variance for easting and northing values for WGS 84. */
	double TranMerc_Delta_Easting;
	double TranMerc_Delta_Northing;

	/* Ellipsoid Parameters, default to WGS 84  */
	double TranMerc_f;      /* Flattening of ellipsoid  */
	double TranMerc_ebs;   /* Second Eccentricity squared */

};

#endif // __SULTRANVERSEMERCATORPARAMETERS_H__