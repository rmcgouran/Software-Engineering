#ifndef ROUTE_H_211217
#define ROUTE_H_211217

#include <string>
#include <vector>
#include <list>

#include "types.h"
#include "position.h"
#include "points.h"

namespace GPS
{
  class Route
  {
    protected:
      std::list<RoutePoint> routePoints;

      /* Class Invariant:
       *   - There is always at least one RoutePoint in the 'routePoints' list - it is never empty.
       */

    public:
      Route(std::vector<RoutePoint>);


      // Returns the number of stored route points.
      unsigned int numPoints() const;


      /* The total length of the Route. This is the sum of the distances between
       * successive route points, including both vertical and horizontal distance.
       */
      metres totalLength() const;


      /* The distance between the first and last points on the Route.
       * This includes both vertical and horizontal distance.
       */
      metres netLength() const;


      /* The sum of all the positive (>0) height differences between successive route points.
       * That is, downhill changes are ignored.
       */
      metres totalHeightGain() const;


      /* The increase in height from the start point to the finishing point.
       * Returns zero if the height difference is negative.
       */
      metres netHeightGain() const;


      /* The steepest uphill gradient (in degrees) between successive points on the route.
       * If the entire route is downhill, then this is the least steep downhill gradient (i.e. negative).
       * Throws a std::domain_error if the route only contains a single route point.
       */
      degrees maxGradient() const;


      /* The steepest downhill gradient (in degrees) between successive points on the Route.
       * If the entire route is uphill, then this is the least steep uphill gradient (i.e. positive).
       * Throws a std::domain_error if the route only contains a single route point.
       */
      degrees minGradient() const;


      /* The steepest gradient between successive points on the Route, whether uphill or downhill
       * (i.e. either positive or negative).
       * Throws a std::domain_error if the route only contains a single route point.
       */
      degrees steepestGradient() const;


      // The point on the Route with the highest elevation.
      RoutePoint highestPoint() const;


      // The point on the Route with the lowest elevation.
      RoutePoint lowestPoint() const;


      // The point on the Route that is farthest north.
      RoutePoint mostNorthelyPoint() const;


      // The point on the Route that is farthest south.
      RoutePoint mostSoutherlyPoint() const;


      // The point on the Route that is farthest east.
      RoutePoint mostEasterlyPoint() const;


      // The point on the Route that is farthest west.
      RoutePoint mostWesterlyPoint() const;


      // The point on the Route that is nearest to the equator.
      RoutePoint mostEquatorialPoint() const;


      // The point on the Route that is farthest from the equator.
      RoutePoint leastEquatorialPoint() const;


      /* Find the Position bearing the specified name.
       * Throws a std::invalid_argument exception if the argument is an empty string.
       * Throws a std::domain_error exception if the name is not found.
       */
      Position findPosition(std::string soughtName) const;


      /* Compute how many times the named route point is visited during the route.
       * Throws a std::invalid_argument exception if the argument is an empty string.
       */
      unsigned int timesVisited(std::string soughtName) const;


      /* Return the route point at the specified index.
       * Throws a std::out_of_range exception if the index is out-of-range.
       */
      RoutePoint operator[](unsigned int) const;


      /* Return the route point that is nearest to the specified Position.
       * Note: this is based only on horizontal distance between locations,
       * (so not taking into account elevation).
       */
      RoutePoint nearestPointTo(Position) const;


      /* Return the route point that is farthest from the specified Position.
       * Note: this is based only on horizontal distance between locations,
       * (so not taking into account elevation).
       */
      RoutePoint farthestPointFrom(Position) const;


    protected:
      Route() = default; // For use by Track subclass
  };
}

#endif
