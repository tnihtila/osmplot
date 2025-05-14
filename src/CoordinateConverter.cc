// Created         : 2014-06-19T14:18:44+0300
// Author          : Timo Nihtila <timo.nihtila@gmail.com>

#include <iostream>
#include <tuple>
#include <cmath>
#include "CoordinateConverter.h"

string CoordinateConverter::zoneLetters_m( "CDEFGHJKLMNPQRSTUVW" );
double CoordinateConverter::firstZone_deg_m = -72;
double CoordinateConverter::lastZone_deg_m = 72;
double CoordinateConverter::zoneStep_deg_m = 8;

CoordinateConverter::CoordinateConverter() :
   equatorialRadius_meters_m( 6378137.0 ),
   polarRadius_meters_m( 6356752.314245 ),
   e2squared_m( 0 ),
   c_m( 0 ),
   alpha_m( 0 ),
   beta_m( 0 ),
   gamma_m( 0 )
{
   e2squared_m = pow( ( sqrt( ( pow( equatorialRadius_meters_m, 2 ) ) -
                              ( pow( polarRadius_meters_m, 2 ) ) ) ) /
                      polarRadius_meters_m, 2 );

   c_m = pow( equatorialRadius_meters_m, 2 ) / polarRadius_meters_m;
   alpha_m = ( 3.0 / 4.0 ) * e2squared_m;
   beta_m =  ( 5.0 / 3.0 ) * pow( alpha_m, 2 );
   gamma_m = ( 35.0 / 27.0 ) * pow( alpha_m, 3 );
}

CoordinateConverter::~CoordinateConverter()
{
}

int CoordinateConverter::Fix( double value )
{
   if( value < 0 )
   {
      return -floor( abs( value ) );
   }
   else
   {
      return floor( value );
   }
}

tuple< double, double, unsigned int, char >
CoordinateConverter::LonLatToUTM( double lon_deg, double lat_deg )
{
   double lat_rad = lat_deg * ( M_PI / 180 );
   double lon_rad = lon_deg * ( M_PI / 180 );

   int UTMZone = Fix( ( lon_deg / 6.0 ) + 31);
   double S = ( ( UTMZone * 6 ) - 183 );
   double deltaS = lon_rad - ( S * ( M_PI / 180 ) );

   char UTMZoneLetter = 'X';

   unsigned int i = 0;
   for( double d = firstZone_deg_m;
        d <= lastZone_deg_m;  d += zoneStep_deg_m, ++i )
   {
      if( lat_deg < d )
      {
         UTMZoneLetter = zoneLetters_m[ i ];
         break;
      }
   }

   double a = cos(lat_rad) * sin(deltaS);
   double epsilon = 0.5 * log( ( 1 + a ) / ( 1 - a ) );
   double nu = atan( tan(lat_rad) / cos(deltaS) ) - lat_rad;
   double v = ( c_m / sqrt( ( 1 + ( e2squared_m * pow( cos(lat_rad), 2 ) ) ) ) ) * 0.9996;
   double ta = pow(( e2squared_m / 2 ) * epsilon, 2 ) * pow(( cos(lat_rad) ), 2 );
   double a1 = sin( 2 * lat_rad );
   double a2 = a1 * pow( cos(lat_rad), 2 );
   double j2 = lat_rad + ( a1 / 2.0 );
   double j4 = ( ( 3 * j2 ) + a2 ) / 4.0;
   double j6 = ( ( 5 * j4 ) + ( a2 * pow( cos(lat_rad), 2 ) ) ) / 3.0;
   double Bm = 0.9996 * c_m * ( lat_rad - alpha_m * j2 + beta_m * j4 - gamma_m * j6 );

   double x = epsilon * v * ( 1 + ( ta / 3.0 ) ) + 500000;
   double y = nu * v * ( 1 + ta ) + Bm;

   if( y < 0 )
   {
      y = 9999999+y;
   }

   return make_tuple( x, y, UTMZone, UTMZoneLetter );
}

double CoordinateConverter::CalculateDistance( double lon1_deg,
                                                 double lat1_deg,
                                                 double lon2_deg,
                                                 double lat2_deg )
{
   double dLat = (lat2_deg-lat1_deg) * M_PI / 180.0;
   double dLon = (lon2_deg-lon1_deg) * M_PI / 180.0;
   double a = sin(dLat/2) * sin(dLat/2) +
      cos(lat1_deg*M_PI/180.0) * cos(lat2_deg*M_PI/180.0) *
      sin(dLon/2) * sin(dLon/2);
   return equatorialRadius_meters_m * 2 * atan2(sqrt(a),sqrt(1-a));
}
