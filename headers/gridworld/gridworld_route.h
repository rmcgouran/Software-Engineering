#ifndef GRIDWORLD_ROUTE_H_120218
#define GRIDWORLD_ROUTE_H_120218

#include <vector>
#include <string>

#include "points.h"
#include "gridworld_model.h"

namespace GridWorld
{
  /* This class generates routes in either:
   *  - GPX format
   *  - NMEA format (currently unimplemented)
   *  - C++ data structure format (std::vector<RoutePoint>)
   *
   * To use this class, the user must provide a string of GridWorld::Points
   * specifying the route path, e.g. "ABCD".
   * The locations of these points are interpreted in a GridWorldModel (see GridWorld.h).
   * The user may provide a GridWorldModel object; if not the default GridWorldModel is used.
   */
  class GridWorldRoute
  {
    public:
      GridWorldRoute(std::string route, // A string of Points.
                     GridWorldModel gridworld = GridWorldModel()); // The default GridWorldModel can be overwritten if desired.

      // Produce a GPX representation of the route.
      std::string toGPX() const;

      // Produce a NMEA representation of the route.
      std::string toNMEA() const; // unimplemented

      // Produce a string representation of the route.
      std::string toString() const;

      // Produce a vector of RoutePoints representation of the route.
      std::vector<GPS::RoutePoint> toRoutePoints() const;

      // Check whether the argument would be a valid string for creating a GridWorldRoute.
      static bool isValidRouteString(std::string);

    private:
      const std::string routeString;
      const GridWorldModel gridworld;

      std::vector<GPS::RoutePoint> routePoints;

      void constructRoutePoints();
  };
}
#endif
