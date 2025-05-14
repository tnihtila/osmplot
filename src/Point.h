#ifndef Point_h
#define Point_h

#include <utility>
#include <tuple>
#include "CoordinateConverter.h"

class Line;
class Point
{
public:
    Point();
    Point(double x, double y);
    Point(std::pair<double, double> data);
    Point(const Point&);
    ~Point();
    double x() const;
    double y() const;
    void x(double x);
    void y(double y);
    double Dot(const Point& b) const;
    double PerpDot(const Point& b) const;
    double DistSqr(const Point& b) const;
    double DistSqr(const Line& line) const;
    Point ProjectionToLine(const Line& line) const;
    Point& operator=(const Point& source);
    Point& operator+(const Point& source);
    Point& operator-(const Point& source);
    bool operator==(const Point& other) const;
    void Convert();

    static int Flip_Y(int y, int height);
    static double WorldWidth();
    static double WorldHeight();
    static void SetWorldLimits(double minlon,
                               double minlat,
                               double maxlon,
                               double maxlat);
private:
    std::pair<double,double> data_m;
    static CoordinateConverter conv_m;
    static std::tuple<double,double,double,double> worldLimits_m;
    static bool limitsConverted_m;
};

#endif // Point_h
