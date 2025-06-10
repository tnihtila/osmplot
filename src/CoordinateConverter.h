// Created         : 2014-06-19T14:18:44+0300
// Author          : Timo Nihtila <timo.nihtila@gmail.com>

#ifndef COORDINATECONVERTER_H
#define COORDINATECONVERTER_H

#include <tuple>
#include <string>

class CoordinateConverter
{
  public:
    CoordinateConverter();
    ~CoordinateConverter();

    std::tuple<double, double, unsigned int, char> LonLatToUTM(double lon_deg,
                                                               double lat_deg);

    double CalculateDistance(double lon1_deg,
                             double lat1_deg,
                             double lon2_deg,
                             double lat2_deg);

  private:
    double equatorialRadius_meters_m;
    double polarRadius_meters_m;
    double e2squared_m;
    double c_m;
    double alpha_m;
    double beta_m;
    double gamma_m;

    static std::string zoneLetters_m;
    static double firstZone_deg_m;
    static double lastZone_deg_m;
    static double zoneStep_deg_m;

    int Fix(double value);

    CoordinateConverter(const CoordinateConverter&);
    CoordinateConverter& operator=(const CoordinateConverter&);
};

#endif // COORDINATECONVERTER_H
