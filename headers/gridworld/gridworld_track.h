#ifndef GRIDWORLD_TRACK_H_120218
#define GRIDWORLD_TRACK_H_120218

#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#include "points.h"

#include "gridworld_model.h"

using std::chrono::system_clock;

namespace GridWorld
{
  /* This class generates tracks in either:
   *  - GPX format
   *  - NMEA format (currently unimplemented)
   *  - C++ data structure format (std::vector<TrackPoint>)
   *
   * To use this class, the user must provide a string of GridWorld::Points,
   * interspersed with time durations (in seconds), specifying the track path and times.
   * E.g. "A1B3C" means that it takes 1 second to travel from Point A to Point B,
   * then 3 seconds to travel from Point B to Point C.
   *
   * The locations of these points are interpreted in a GridWorldModel (see GridWorld.h).
   * The user may provide a GridWorldModel object; if not the default GridWorldModel is used.
   */
  class GridWorldTrack
  {      
    public:
      GridWorldTrack(std::string track, // A string containing Points and time units.
                     GridWorldModel gridworld = GridWorldModel(), // The default GridWorldModel can be overwritten if desired.
                     system_clock::time_point startTime = {}); // The time point of the first track point.

      // Produce a GPX representation of the track.
      std::string toGPX() const;

      // Produce a NMEA representation of the track.
      std::string toNMEA() const; // unimplemented

      // Produce a string representation of the track.
      std::string toString() const;

      // Produce a vector of TrackPoints representation of the route.
      std::vector<GPS::TrackPoint> toTrackPoints() const;

      // Check whether the argument would be a valid string for creating a GridWorldTrack.
      static bool isValidTrackString(std::string);

      // Convert a track string into a route string (by discarding the timing information).
      static std::string routeStringFromTrackString(std::string);

    private:
      const std::string trackString;
      const GridWorldModel gridworld;
      const system_clock::time_point startTime;

      std::vector<GPS::TrackPoint> trackPoints;

      void constructTrackPoints();

      static std::tm time_pointTotm(system_clock::time_point);
      static std::string dateTimeToString(const std::tm&);
  };
}

#endif
