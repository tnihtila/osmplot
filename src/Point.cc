// Created         : 2014-06-19T11:27:43+0300
// Author          : Timo Nihtila <timo.nihtila@gmail.com>

#include "Point.h"
#include "Line.h"
#include <math.h>
#include <iostream>

std::tuple<double,double,double,double> Point::worldLimits_m(999999,999999,-999999,-999999);
CoordinateConverter Point::conv_m;
bool Point::limitsConverted_m = false;

using namespace std;

Point::Point() :
   data_m(0,0)
{
}

Point::Point(double x,
             double y) :
   data_m(x, y)
{
}

Point::Point(std::pair<double,double> data) :
   data_m(data)
{
}

Point::~Point()
{
}

Point::Point(const Point& source):
   data_m(source.data_m)
{
}

void Point::Convert()
{
   if (!limitsConverted_m)
   {
      auto minTmp = conv_m.LonLatToUTM( get<0>(worldLimits_m),
                                        get<1>(worldLimits_m) );
      get<0>(worldLimits_m) = get<0>(minTmp);
      get<1>(worldLimits_m) = get<1>(minTmp);
      auto maxTmp = conv_m.LonLatToUTM( get<2>(worldLimits_m),
                                        get<3>(worldLimits_m) );
      get<2>(worldLimits_m) = get<0>(maxTmp);
      get<3>(worldLimits_m) = get<1>(maxTmp);
      limitsConverted_m = true;
   }

   auto tmp = conv_m.LonLatToUTM( data_m.first, data_m.second );
   data_m.first = get<0>(tmp) - get<0>(worldLimits_m);
   data_m.second = get<1>(tmp) - get<1>(worldLimits_m);
}

Point& Point::operator=(const Point& source)
{
   if (this != &source)
   {
      data_m = source.data_m;
   }
   return *this;
}

Point& Point::operator+(const Point& source)
{
   data_m.first += source.data_m.first;
   data_m.second += source.data_m.second;
   return *this;
}

Point& Point::operator-(const Point& source)
{
   data_m.first -= source.data_m.first;
   data_m.second -= source.data_m.second;
   return *this;
}

double Point::x() const
{
   return data_m.first;
}

double Point::y() const
{
   return data_m.second;
}

double Point::Dot(const Point& b) const
{
   return (data_m.first*b.x()) + (data_m.second*b.y());
}

double Point::PerpDot(const Point& b) const
{
   return (data_m.second*b.x()) - (data_m.first*y());
}

double Point::DistSqr(const Point& b) const
{
   return pow((data_m.first-b.x()),2)+pow((data_m.second-b.y()),2);
}

Point Point::ProjectionToLine(const Line& line) const
{
   // Return minimum distance between line segment vw and point p
   double l2 = line.a().DistSqr(line.b()); //length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt

   if (l2 == 0.0)
   {
      return Point(line.a());   // v == w case
   }

   // Consider the line extending the segment, parameterized as v + t (w - v).
   // We find projection of point p onto the line.
   // It falls where t = [(p-v) . (w-v)] / |w-v|^2
   double t = ((data_m.first - line.a().x()) * (line.b().x() - line.a().x())
               + (data_m.second - line.a().y()) * (line.b().y() - line.a().y())) / l2;

   if (t < 0.0)
   {
      return Point(line.a());       // Beyond the 'v' end of the segment
   }
   else if (t > 1.0)
   {
      return Point(line.b()); // Beyond the 'w' end of the segment
   }

   return Point(line.a().x() + t * (line.b().x() - line.a().x()),
                line.a().y() + t * (line.b().y() - line.a().y()));
}

double Point::DistSqr(const Line& line) const
{
   return DistSqr(ProjectionToLine(line));
}

bool Point::operator==(const Point& other) const
{
   return DistSqr(other) < 0.01;
}

int Point::Flip_Y(int y, int height)
{
   return height - y;
}

double Point::WorldWidth()
{
    return get<2>(worldLimits_m)-get<0>(worldLimits_m);
}

double Point::WorldHeight()
{
    return get<3>(worldLimits_m)-get<1>(worldLimits_m);
}

void Point::SetWorldLimits(double minlon,
                           double minlat,
                           double maxlon,
                           double maxlat)
{
    get<0>(worldLimits_m) = minlon;
    get<1>(worldLimits_m) = minlat;
    get<2>(worldLimits_m) = maxlon;
    get<3>(worldLimits_m) = maxlat;
}
