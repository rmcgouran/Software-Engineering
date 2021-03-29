#ifndef PARSEGPX_H_201220
#define PARSEGPX_H_201220

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>

// Include of header files
#include "points.h"
#include "xml/parser.h"

namespace GPX
{
  // Namespaces for use within code
  using namespace GPS;
  using namespace XML;
  using namespace boost::algorithm;

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
