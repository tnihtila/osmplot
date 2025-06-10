// Created         : 2014-06-19T12:37:53+0300
// Author          : Timo Nihtila <timo.nihtila@gmail.com>

#ifndef LINE_H
#define LINE_H

#include "Point.h"
#define cimg_display 1
#include "CImg.h"

class Line
{
  public:
    Line(const Point& a, const Point& b);
    Line(const Line& source);
    ~Line();
    Line& operator=(const Line& source);
    bool operator==(const Line& other) const;
    const Point& a() const;
    const Point& b() const;
    void SetWidth(double width);
    double Width() const;
    bool Intersects(const Line& line);
    void SetOpen(bool open);
    bool IsOpen() const;
    void Draw(cimg_library::CImg<double>& image, int col[]);
    void DrawWidth(cimg_library::CImg<double>& image, int col[]);

  private:
    void CalcLength();
    std::pair<Point, Point> data;
    double width_m;
    double length_m;
    bool isOpen_m;
};

#endif // LINE_H
