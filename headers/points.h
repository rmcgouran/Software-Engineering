#ifndef POINTS_H_201230
#define POINTS_H_201230

#include <string>
#include <ctime>

#include "position.h"

namespace GPS
{
  struct RoutePoint
  {
     GPS::Position position;
     std::string name;
  };

  struct TrackPoint
  {
     GPS::Position position;
     std::string name;
     std::tm dateTime;
  };
}

#endif
