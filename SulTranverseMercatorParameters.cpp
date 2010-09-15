// SulTranverseMercatorParameters.cpp

#include "stdafx.h"
#include "SulTranverseMercatorParameters.h"

const double MIN_LAT = ( (-80.5 * osg::PI) / 180.0 ); /* -80.5 degrees in radians    */
const double MAX_LAT = ( (84.5 * osg::PI) / 180.0 );  /* 84.5 degrees in radians     */
const double MAX_DELTA_LONG  = ((osg::PI * 90)/180.0);    /* 90 degrees in radians */

const double flatteningReciprocal = 298.257223563;
const double Geocent_f = 1 / flatteningReciprocal; /* Flattening of ellipsoid           */

CSulTranverseMercatorParameters::CSulTranverseMercatorParameters()
{
  TranMerc_a = 6378137.0;         /* Semi-major axis of ellipsoid i meters */
  TranMerc_es = 0.0066943799901413800; /* Eccentricity (0.08181919084262188000) squared */

  /* Isometeric to geodetic latitude parameters, default to WGS 84 */
  TranMerc_ap = 6367449.1458008;
  TranMerc_bp = 16038.508696861;
  TranMerc_cp = 16.832613334334;
  TranMerc_dp = 0.021984404273757;
  TranMerc_ep = 3.1148371319283e-005;

  /* Transverse_Mercator projection Parameters */
  TranMerc_Origin_Lat = 0.0;           /* Latitude of origin in radians */
  TranMerc_Origin_Long = 0.0;          /* Longitude of origin in radians */
  TranMerc_False_Northing = 0.0;       /* False northing in meters */
  TranMerc_False_Easting = 0.0;        /* False easting in meters */
  TranMerc_Scale_Factor = 1.0;         /* Scale factor  */

  /* Maximum variance for easting and northing values for WGS 84. */
  TranMerc_Delta_Easting = 40000000.0;
  TranMerc_Delta_Northing = 40000000.0;

  /* Ellipsoid Parameters, default to WGS 84  */
  TranMerc_f = Geocent_f;      /* Flattening of ellipsoid  */
  TranMerc_ebs = 0.0067394967565869;   /* Second Eccentricity squared */
}

double CSulTranverseMercatorParameters::SPHTMD(double Latitude) const
{
  return ((double) (TranMerc_ap * Latitude
     - TranMerc_bp * sin(2.e0 * Latitude) + TranMerc_cp * sin(4.e0 * Latitude)
     - TranMerc_dp * sin(6.e0 * Latitude) + TranMerc_ep * sin(8.e0 * Latitude)));
}

double CSulTranverseMercatorParameters::SPHSN(double Latitude) const
{
  return ((double) (TranMerc_a / sqrt( 1.e0 - TranMerc_es * pow(sin(Latitude), 2))));
}

double CSulTranverseMercatorParameters::DENOM(double Latitude) const
{
  return ((double) (sqrt(1.e0 - TranMerc_es * pow(sin(Latitude),2))));
}

double CSulTranverseMercatorParameters::SPHSR(double Latitude) const
{
  return ((double) (TranMerc_a * (1.e0 - TranMerc_es) / pow(DENOM(Latitude), 3)));
}

void CSulTranverseMercatorParameters::calcTransverseMercatorParameters(
	double a, 
	double f, 
	double Origin_Latitude,
	double Central_Meridian, 
	double False_Easting,
	double False_Northing, 
	double Scale_Factor )
{ 
	double tn;        /* True Meridianal distance constant  */
	double tn2;
	double tn3;
	double tn4;
	double tn5;
	double dummy_northing;
	double TranMerc_b; /* Semi-minor axis of ellipsoid, in meters */

	TranMerc_a = a;
	TranMerc_f = f;
	TranMerc_Origin_Lat = 0;
	TranMerc_Origin_Long = 0;
	TranMerc_False_Northing = 0;
	TranMerc_False_Easting = 0;
	TranMerc_Scale_Factor = 1;

	/* Eccentricity Squared */
	TranMerc_es = 2 * TranMerc_f - TranMerc_f * TranMerc_f;
	/* Second Eccentricity Squared */
	TranMerc_ebs = (1 / (1 - TranMerc_es)) - 1;

	TranMerc_b = TranMerc_a * (1 - TranMerc_f);
	/*True meridianal constants  */
	tn = (TranMerc_a - TranMerc_b) / (TranMerc_a + TranMerc_b);
	tn2 = tn * tn;
	tn3 = tn2 * tn;
	tn4 = tn3 * tn;
	tn5 = tn4 * tn;

	TranMerc_ap = TranMerc_a * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
							  + 81.e0 * (tn4 - tn5)/64.e0 );
	TranMerc_bp = 3.e0 * TranMerc_a * (tn - tn2 + 7.e0 * (tn3 - tn4)
									 /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
	TranMerc_cp = 15.e0 * TranMerc_a * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
	TranMerc_dp = 35.e0 * TranMerc_a * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
	TranMerc_ep = 315.e0 * TranMerc_a * (tn4 - tn5) / 512.e0;
	convertGeodeticToTransverseMercator(*this, MAX_LAT, MAX_DELTA_LONG, TranMerc_Delta_Easting,
												   TranMerc_Delta_Northing);
	convertGeodeticToTransverseMercator(*this, 0, MAX_DELTA_LONG, TranMerc_Delta_Easting,
												   dummy_northing);
	TranMerc_Origin_Lat = Origin_Latitude;
	if (Central_Meridian > osg::PI)
	 Central_Meridian -= (2*osg::PI);
	TranMerc_Origin_Long = Central_Meridian;
	TranMerc_False_Northing = False_Northing;
	TranMerc_False_Easting = False_Easting;
	TranMerc_Scale_Factor = Scale_Factor;
}

void CSulTranverseMercatorParameters::convertGeodeticToTransverseMercator(
	const CSulTranverseMercatorParameters& params,
    double Latitude,
    double Longitude,
    double& Easting,
    double& Northing )
{      /* BEGIN Convert_Geodetic_To_Transverse_Mercator */
double c;       /* Cosine of latitude                          */
      double c2;
      double c3;
      double c5;
      double c7;
      double dlam;    /* Delta longitude - Difference in Longitude       */
      double eta;     /* constant - TranMerc_ebs *c *c                   */
      double eta2;
      double eta3;
      double eta4;
      double s;       /* Sine of latitude                        */
      double sn;      /* Radius of curvature in the prime vertical       */
      double t;       /* Tangent of latitude                             */
      double tan2;
      double tan3;
      double tan4;
      double tan5;
      double tan6;
      double t1;      /* Term in coordinate conversion formula - GP to y */
      double t2;      /* Term in coordinate conversion formula - GP to y */
      double t3;      /* Term in coordinate conversion formula - GP to y */
      double t4;      /* Term in coordinate conversion formula - GP to y */
      double t5;      /* Term in coordinate conversion formula - GP to y */
      double t6;      /* Term in coordinate conversion formula - GP to y */
      double t7;      /* Term in coordinate conversion formula - GP to y */
      double t8;      /* Term in coordinate conversion formula - GP to y */
      double t9;      /* Term in coordinate conversion formula - GP to y */
      double tmd;     /* True Meridional distance                        */
      double tmdo;    /* True Meridional distance for latitude of origin */
      double temp_Origin;
      double temp_Long;

      if (Longitude > osg::PI)
        Longitude -= (2 * osg::PI);
      if ((Longitude < (params.TranMerc_Origin_Long - MAX_DELTA_LONG))
          || (Longitude > (params.TranMerc_Origin_Long + MAX_DELTA_LONG)))
      {
         if (Longitude < 0)
           temp_Long = Longitude + 2 * osg::PI;
         else
           temp_Long = Longitude;
         if (params.TranMerc_Origin_Long < 0)
           temp_Origin = params.TranMerc_Origin_Long + 2 * osg::PI;
         else
           temp_Origin = params.TranMerc_Origin_Long;
      }

      /*
       *  Delta Longitude
       */
      dlam = Longitude - params.TranMerc_Origin_Long;

      if (dlam > osg::PI)
        dlam -= (2 * osg::PI);
      if (dlam < -osg::PI)
        dlam += (2 * osg::PI);
      if (fabs(dlam) < 2.e-10)
        dlam = 0.0;

      s = sin(Latitude);
      c = cos(Latitude);
      c2 = c * c;
      c3 = c2 * c;
      c5 = c3 * c2;
      c7 = c5 * c2;
      t = tan (Latitude);
      tan2 = t * t;
      tan3 = tan2 * t;
      tan4 = tan3 * t;
      tan5 = tan4 * t;
      tan6 = tan5 * t;
      eta = params.TranMerc_ebs * c2;
      eta2 = eta * eta;
      eta3 = eta2 * eta;
      eta4 = eta3 * eta;

      /* radius of curvature in prime vertical */
      sn = params.SPHSN(Latitude);

      /* True Meridianal Distances */
      tmd = params.SPHTMD(Latitude);

      /*  Origin  */
      tmdo = params.SPHTMD(params.TranMerc_Origin_Lat);

      /* northing */
      t1 = (tmd - tmdo) * params.TranMerc_Scale_Factor;
      t2 = sn * s * c * params.TranMerc_Scale_Factor/ 2.e0;
      t3 = sn * s * c3 * params.TranMerc_Scale_Factor * (5.e0 - tan2 + 9.e0 * eta
                                                  + 4.e0 * eta2) /24.e0;

      t4 = sn * s * c5 * params.TranMerc_Scale_Factor * (61.e0 - 58.e0 * tan2
                                                  + tan4 + 270.e0 * eta - 330.e0 * tan2 * eta + 445.e0 * eta2
                                                  + 324.e0 * eta3 -680.e0 * tan2 * eta2 + 88.e0 * eta4
                                                  -600.e0 * tan2 * eta3 - 192.e0 * tan2 * eta4) / 720.e0;

      t5 = sn * s * c7 * params.TranMerc_Scale_Factor * (1385.e0 - 3111.e0 *
                                                  tan2 + 543.e0 * tan4 - tan6) / 40320.e0;

      Northing = params.TranMerc_False_Northing + t1 + pow(dlam,2.e0) * t2
        + pow(dlam,4.e0) * t3 + pow(dlam,6.e0) * t4
        + pow(dlam,8.e0) * t5;

      /* Easting */
      t6 = sn * c * params.TranMerc_Scale_Factor;
      t7 = sn * c3 * params.TranMerc_Scale_Factor * (1.e0 - tan2 + eta ) /6.e0;
      t8 = sn * c5 * params.TranMerc_Scale_Factor * (5.e0 - 18.e0 * tan2 + tan4
                                              + 14.e0 * eta - 58.e0 * tan2 * eta + 13.e0 * eta2 + 4.e0 * eta3
                                              - 64.e0 * tan2 * eta2 - 24.e0 * tan2 * eta3 )/ 120.e0;
      t9 = sn * c7 * params.TranMerc_Scale_Factor * ( 61.e0 - 479.e0 * tan2
                                               + 179.e0 * tan4 - tan6 ) /5040.e0;

      Easting = params.TranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7
        + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;
}
