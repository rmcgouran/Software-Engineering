#ifndef TRACK_H_211217
#define TRACK_H_211217

#include <string>
#include <vector>
#include <list>
#include <ctime>
#include <chrono>

#include "types.h"
#include "position.h"
#include "points.h"
#include "route.h"

namespace GPS
{
  class Track : public Route
  {
    protected:
      /* For each track point, we store a corresponding time stamp recording
       * the arrival time and departure time from that point.
       */
      struct TimeStamp
      {
          std::chrono::system_clock::time_point arrival;
          std::chrono::system_clock::time_point departure;
          // TODO: C++20 provides a dedicated gps_clock.
      };

    std::list<TimeStamp> timeStamps;
    /* Class Invariant:
     *   The length of the 'timeStamps' list is the same as that of the 'routePoints' list.
     */


    /* A Track has a configurable 'granularity' value, which is the minimum horizontal distance
     * between two successive points. Points closer than this are merged. This merging is what
     * can lead to different arrival and departure time stamps for a track point.
     */
    metres granularity = 0;


    public:
      /*  The 'granularity' parameter is the minimum distance between successive route points.
       *  Any route points closer to their predecessor than this are discarded.
       */
      Track(std::vector<TrackPoint>, metres granularity = 10);


      /* Update the granularity of the stored track.  Any position in the track that differs in
       * distance from its predecessor by less than the updated granularity is discarded.
       */
      void setGranularity(metres);


      // Total elapsed time between start and finish of track.
      std::chrono::seconds totalTime() const;


      // Total elapsed time between start and finish of the track that is spent moving.
      std::chrono::seconds travellingTime() const;


      // Total elapsed time between start and finish of the track that is spent not moving.
      std::chrono::seconds restingTime() const;


      // The duration of the longest period spent not moving during the track.
      std::chrono::seconds longestRest() const;


      /* The fastest speed between successive track points.
       * Returns zero if the track only contains one point.
       * Throws a std::domain_error if the time between two successive points is zero (or negative).
       */
       speed maxSpeed() const;


      /* The average speed over the course of the whole track.
       * The parameter controls whether resting time should be included in the average or not.
       * Returns zero if the track only contains one point.
       * Throws a std::domain_error if the total (included) time is zero (or negative).
       */
      speed averageSpeed(bool includeRests) const;


      // The fastest rate of positive elevation change between successive track points.
      // Returns zero if there are no positive elevation changes.
      // Throws a std::domain_error if the time between two successive points is zero (or negative).
      speed maxRateOfAscent() const;


      /* The fastest rate of negative elevation change between successive track points
       * (returned as a positive number).
       * Returns zero if there are no negative elevation changes.
       * Throws a std::domain_error if the time between two successive points is zero (or negative).
       */
      speed maxRateOfDescent() const;


      /* Returns the name of the first track point within 'granularity' of the specified Position.
       * Returns "Unnamed Position" if the point is not named.
       * Throws a std::domain_error exception if the specified Position is not found.
       */
      std::string findNameOf(Position) const;


      // Compute how many track points are within 'granularity' of the specified Position.
      unsigned int timesVisited(Position) const;


      /* Determine if any point in the track is within 'granularity' of another point in the track.
       */
      bool containsCycles() const;


    private:

      /* Two Positions are considered to be the same location if they are less than
       * "granularity" metres apart (horizontally).
       */
      bool areSameLocation(Position,Position) const;

      static TimeStamp tmToTimeStamp(std::tm);
  };
}

#endif
