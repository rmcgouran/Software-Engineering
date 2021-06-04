#ifndef PARSEGPX_H_201220
#define PARSEGPX_H_201220

#include <string>
#include <vector>

#include "points.h"

namespace GPX
{
  /*  Parse GPX data containing a route.
   *  The source data can be provided as a string, or from a file; which one is determined by the bool parameter.
   */
  std::vector<GPS::RoutePoint> parseRoute(std::string source, bool isFileName);


  /*  Parse GPX data containing a track.
   *  The source data can be provided as a string, or from a file; which one is determined by the bool parameter.
   */
  std::vector<GPS::TrackPoint> parseTrack(std::string source, bool isFileName);
}

#endif
