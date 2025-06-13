// Created         : 2014-06-19T14:18:44+0300
// Author          : Timo Nihtila <timo.nihtila@gmail.com>

#include "Point.h"
#include "Line.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <memory>
#include <set>

#define cimg_display 1
#include "CImg.h"

using namespace std;
using namespace cimg_library;

int white[] = {255,255,255};
int blue[] = {100,100,255};
int green[] = {100,255,100};
int red[] = {255,100,100};
int black[] = {0,0,0};

double laneWidth_meters = 5.0;

map<std::string,map<std::string,double>> ways;
set<vector<std::string>> buildings;
map<std::string,shared_ptr<Point>> nodes;

vector<std::string> SplitString(std::string str, char delim = ' ')
{
    vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (getline(ss, token, delim))
    {
        if (token.size())
        {
            tokens.push_back(token);
        }
    }

    return tokens;
}

// Helper: Detects the delimiter used in the line
char DetectDelimiter(const std::string &line)
{
    if (line.find("\"") != std::string::npos)
    {
        return '\"';
    }
    else if (line.find("\'") != std::string::npos)
    {
        return '\'';
    }    
    return ' '; // Default fallback
}

// Helper: Parses <bounds> tag
void ParseBounds(const std::string &line, char delim)
{
    vector<std::string> tokens = SplitString(line);
    std::string minlat, minlon, maxlat, maxlon;

    for (auto &it : tokens)
    {
        vector<std::string> tokens1 = SplitString(it, delim);
        if (it.find("minlat=") != std::string::npos)
        {
            minlat = tokens1[1];
        }
        else if (it.find("minlon=") != std::string::npos)
        {
            minlon = tokens1[1];
        }
        else if (it.find("maxlat=") != std::string::npos)
        {
            maxlat = tokens1[1];
        }
        else if (it.find("maxlon=") != std::string::npos)
        {
            maxlon = tokens1[1];
        }
    }

    Point::SetWorldLimits(stod(minlon), stod(minlat), stod(maxlon), stod(maxlat));
}

// Helper: Parses <node> tag
void ParseNode(const std::string &line, char delim)
{
    vector<std::string> tokens = SplitString(line);
    std::string id, lat, lon;

    for (auto &it : tokens)
    {
        vector<std::string> tokens1 = SplitString(it, delim);
        if (it.find("id=") != std::string::npos && it.find("uid=") == std::string::npos)
        {
            id = tokens1[1];
        }
        else if (it.find("lat=") != std::string::npos)
        {
            lat = tokens1[1];
        }
        else if (it.find("lon=") != std::string::npos)
        {
            lon = tokens1[1];
        }
    }

    nodes[id] = make_shared<Point>(stod(lon), stod(lat));
}

// Helper: Extracts node references from a line
void ExtractNodeRefs(const std::string &line, char delim, vector<std::string> &noderefs)
{
    vector<std::string> tokens = SplitString(line);
    for (auto &it : tokens)
    {
        if (it.find("ref=") != std::string::npos)
        {
            vector<std::string> tokens1 = SplitString(it, delim);
            noderefs.push_back(tokens1[1]);
        }
    }
}

// Helper: Parses tags inside <way> tag
void ParseWayTags(const std::string &line, char delim, bool &isBuilding, bool &isWay)
{
    vector<std::string> tokens = SplitString(line);
    for (auto &it : tokens)
    {
        vector<std::string> tokens1 = SplitString(it, delim);
        if (it.find("k=") != std::string::npos)
        {
            if (tokens1[1] == "building")
            {
                isBuilding = true;
            }
            else if (tokens1[1] == "highway")
            {
                isWay = true;
            }
        }
    }
}

// Helper: Adds a way to the global map
void AddWay(const vector<std::string> &noderefs)
{
    for (size_t i = 0; i < noderefs.size() - 1; ++i)
    {
        ways[noderefs[i]][noderefs[i + 1]] = laneWidth_meters;
        ways[noderefs[i + 1]][noderefs[i]] = laneWidth_meters;
    }
}

// Helper: Parses <way> tag and its related content
void ParseWay(ifstream &file, std::string &line, char delim)
{
    bool isBuilding = false, isWay = false;
    vector<std::string> noderefs;
    std::string id;

    vector<std::string> tokens = SplitString(line);
    for (auto &it : tokens)
    {
        if (it.find("id=") != std::string::npos && it.find("uid=") == std::string::npos)
        {
            vector<std::string> tokens1 = SplitString(it, delim);
            id = tokens1[1];
        }
    }

    while (getline(file, line))
    {
        if (line.find("way>") != std::string::npos)
        {
            if (!noderefs.empty())
            {
                if (isBuilding)
                {
                    buildings.insert(noderefs);
                }
                else if (isWay)
                {
                    AddWay(noderefs);
                }
            }
            break;
        }
        else if (line.find("<nd ") != std::string::npos)
        {
            ExtractNodeRefs(line, delim, noderefs);
        }
        else if (line.find("<tag ") != std::string::npos)
        {
            ParseWayTags(line, delim, isBuilding, isWay);
        }
    }
}

void ParseFile(const std::string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    std::string line;
    while (getline(file, line))
    {
        char delim = DetectDelimiter(line);

        if (line.find("<bounds") != std::string::npos)
        {
            ParseBounds(line, delim);
        }
        else if (line.find("<node") != std::string::npos)
        {
            ParseNode(line, delim);
        }
        else if (line.find("<way") != std::string::npos)
        {
            ParseWay(file, line, delim);
        }
    }

    // Convert all nodes after parsing
    for (auto &it : nodes)
    {
        it.second->Convert();
    }

    file.close();
}

void PlotBuildings(CImg<double>& image)
{
    for (auto &it : buildings)
    {
        CImg<> points(it.size(),2);
        for (unsigned int i = 0; i < it.size(); ++i)
        {
            points(i,0) =  static_cast<int>(image.width() * nodes[it[i]]->x() / Point::WorldWidth());
            int tmp = static_cast<int>(image.height() * nodes[it[i]]->y() / Point::WorldHeight());
            points(i,1) = Point::Flip_Y(tmp,image.height());

            if (i)
            {
                image.draw_line(points(i-1,0),points(i-1,1),points(i,0),points(i,1),white,0.75);
            }
            image.draw_circle(points(i,0),points(i,1),static_cast<int>(image.height()/3000.0),white,1);

        }
        image.draw_polygon(points,white,0.35);
    }
}

void PlotWays(CImg<double>& image)
{
    for (auto &it : ways)
    {
        for (auto &it1 : it.second)
        {
            int x0 = static_cast<int>(image.width() * nodes[it.first]->x() / Point::WorldWidth());
            int tmp = static_cast<int>(image.height() * nodes[it.first]->y() / Point::WorldHeight());
            int y0 = Point::Flip_Y(tmp, image.height());
            int x1 = static_cast<int>(image.width() * nodes[it1.first]->x() / Point::WorldWidth());
            int y1 = Point::Flip_Y(static_cast<int>(image.height() * nodes[it1.first]->y() / Point::WorldHeight()),image.height());
            
            image.draw_line(x0,y0,x1,y1,blue);
            image.draw_circle(x0,y0,static_cast<int>(image.height()/1000.0),blue,1,1);
            image.draw_circle(x1,y1,static_cast<int>(image.height()/1000.0),blue,1,1);
        }
    }
}

// Function plots lanes of maximum width around the way lines,
// but avoids plotting the lane polygon over any buildings beside the way
void Lanes(CImg<double>& image)
{
   unsigned int k = 0;
   for (auto &src : ways)
   {
      for (auto &dst : src.second)
      {
         Line path(*nodes[src.first],
                   *nodes[dst.first]);

         // Find the minimum distance between any building and the way
         double minDist = 1e37;
         for (auto &bld : buildings)
         {
            double dist;
            for (auto i = 0; i < bld.size()-1; ++i)
            {
               dist = sqrt(nodes[bld[i]]->DistSqr(path));
               if (dist < minDist)
               {
                  minDist = dist;
               }

               Line vertice(*nodes[bld[i]], *nodes[bld[i+1]]);

               minDist = min(sqrt(path.a().DistSqr(vertice)),minDist);
               minDist = min(sqrt(path.b().DistSqr(vertice)),minDist);
            }
         }

         dst.second = min(dst.second, minDist);

         CImg<> points(4,2);

         double xdf = nodes[dst.first]->x() - nodes[src.first]->x();
         double ydf = nodes[dst.first]->y() - nodes[src.first]->y();
         double lengthFactor = dst.second / sqrt(nodes[src.first]->DistSqr(*nodes[dst.first]));

         double x = nodes[src.first]->x() + lengthFactor * ydf;
         double y = nodes[src.first]->y() - lengthFactor * xdf;

         points(0,0) = static_cast<int>(image.width() * nodes[src.first]->x() / Point::WorldWidth());
         points(0,1) = Point::Flip_Y(static_cast<int>(image.height() * nodes[src.first]->y() / Point::WorldHeight()), image.height());

         points(1,0) = static_cast<int>(image.width() * x / Point::WorldWidth());
         points(1,1) = Point::Flip_Y(static_cast<int>(image.height() * y / Point::WorldHeight()),image.height());

         points(2,0) = static_cast<int>(image.width() * (x+xdf) / Point::WorldWidth());
         points(2,1) = Point::Flip_Y(static_cast<int>(image.height() * (y+ydf) / Point::WorldHeight()),image.height());

         points(3,0) = static_cast<int>(image.width() * nodes[dst.first]->x() / Point::WorldWidth());
         points(3,1) = Point::Flip_Y(static_cast<int>(image.height() * nodes[dst.first]->y() / Point::WorldHeight()),image.height());

         image.draw_polygon(points,green,0.25);

         ++k;
      }
   }
}

int main(int argc, char* argv[] )
{
    if ( argc < 2 )
    {
        cout << "Not enough arguments!" << endl;
        return 1;
    }
    
    bool plotBuildings = true;
    bool plotWays = true;
    bool plotLanes = true;
    
    ParseFile(std::string(argv[1]));
    
    double scale = Point::WorldWidth() / Point::WorldHeight();
    
    int maxWidth = 1500 * scale;
    int maxHeight = 1500;
    
    CImg<double> image(maxWidth, maxHeight, 1, 3, 1);
    
    if (plotBuildings)
    {
        PlotBuildings(image);
    }
    
    if (plotWays)
    {
        PlotWays(image);
    }
    
    if (plotLanes)
    {
        Lanes(image);
    }
    
    CImgDisplay disp(image, "Map");
    disp.move(500, 200);
    disp.show();
    
    while(true)
    {
        CImgDisplay::wait(disp);
    }
}
