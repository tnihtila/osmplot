// Created         : 2014-06-19T12:38:13+0300
// Author          : Timo Nihtila <timo.nihtila@gmail.com>

#include "Line.h"
#include "Point.h"
#include <algorithm>

using namespace cimg_library;

Line::Line(const Point& a,
           const Point& b):
   data(a,b),
   width_m(0),
   length_m(0)
{
   CalcLength();
}

Line::~Line()
{
}

Line::Line(const Line& source):
   data(source.data),
   width_m(source.width_m),
   length_m(source.length_m),
   isOpen_m(source.isOpen_m)
{
}

void Line::CalcLength()
{
   length_m = sqrt(a().DistSqr(b()));
}

void Line::SetWidth(double width)
{
   width_m = width;
}

double Line::Width() const
{
   return width_m;
}

const Point& Line::a() const
{
   return data.first;
}

const Point& Line::b() const
{
   return data.second;
}

Line& Line::operator=(const Line& source)
{
   if (this != &source)
   {
      data = source.data;
   }
   return *this;
}

bool Line::operator==(const Line &other) const
{
   return (data == other.data);
}

bool Line::Intersects(const Line& line)
{
   Point u = b();
   u = u-a();

   Point v = line.b();
   v = v-line.a();

   double f = u.PerpDot(v);

   if(!f)
   {
      // lines are parallel
      return false;
   }

   auto c = line.b();
   c = c-b();

   double aa = u.PerpDot(c);
   double bb = v.PerpDot(c);

   if(f < 0)
   {
      if(aa > 0) return false;
      if(bb > 0) return false;
      if(aa < f) return false;
      if(bb < f) return false;
   }
   else
   {
      if(aa < 0) return false;
      if(bb < 0) return false;
      if(aa > f) return false;
      if(bb > f) return false;
   }

   return true;
}

void Line::SetOpen(bool open)
{
   isOpen_m = open;
}

bool Line::IsOpen() const
{
   return isOpen_m;
}

void Line::Draw(CImg<double>& image,
                int col[])
{
   int x0 = static_cast<int>(image.width() * a().x() / Point::WorldWidth());
   int y0 = static_cast<int>(Point::Flip_Y(image.height() * a().y() / Point::WorldHeight(),image.height()));
   int x1 = static_cast<int>(image.width() * b().x() / Point::WorldWidth());
   int y1 = static_cast<int>(Point::Flip_Y(image.height() * b().y() / Point::WorldHeight(),image.height()));

   image.draw_line(x0,y0,x1,y1,col);
   image.draw_circle(x0,y0,static_cast<int>(image.height()/1000.0),col,1,1);
   image.draw_circle(x1,y1,static_cast<int>(image.height()/1000.0),col,1,1);
}

void Line::DrawWidth(CImg<double>& image,
                     int col[])
{
   CImg<> points(4,2);

   double udf = b().x() - a().x();
   double vdf = b().y() - a().y();
   double lengthFactor = width_m / length_m;

   double u = a().x() + lengthFactor * vdf;
   double v = a().y() - lengthFactor * udf;

   points(0,0) = std::max(static_cast<int>(0),
                     std::min(static_cast<int>(image.width()),
                         static_cast<int>(image.width() * a().x() / Point::WorldWidth())));
   points(0,1) = std::max(static_cast<int>(0),
                     std::min(static_cast<int>(image.height()),
                         static_cast<int>(Point::Flip_Y(image.height() * a().y() / Point::WorldHeight(),image.height()))));

   points(1,0) = std::max(static_cast<int>(0),
                     std::min(static_cast<int>(image.width()),
                         static_cast<int>(image.width() * u / Point::WorldWidth())));
   points(1,1) = std::max(static_cast<int>(0),
                     std::min(static_cast<int>(image.height()),
                         static_cast<int>(Point::Flip_Y(image.height() * v / Point::WorldHeight(),image.height()))));

   points(2,0) = std::max(static_cast<int>(0),
                     std::min(static_cast<int>(image.width()),
                              static_cast<int>(image.width() * (u+udf) / Point::WorldWidth())));
   points(2,1) = std::max(static_cast<int>(0),
                          std::min(static_cast<int>(image.height()),
                                   static_cast<int>(Point::Flip_Y(image.height() * (v+vdf) / Point::WorldHeight(),image.height()))));

   points(3,0) = std::max(static_cast<int>(0),
                          std::min(static_cast<int>(image.width()),
                         static_cast<int>(image.width() * a().x() / Point::WorldWidth())));
   points(3,1) = std::max(static_cast<int>(0),
                     std::min(static_cast<int>(image.height()),
                         static_cast<int>(Point::Flip_Y(image.height() * a().y() / Point::WorldHeight(),image.height()))));

   image.draw_polygon(points,col,0.25);

}
