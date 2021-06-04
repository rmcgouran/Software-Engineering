#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <sstream>

#include "geometry.h"
#include "xml/generator.h"
#include "gridworld_model.h"
#include "gridworld_route.h"
#include "gridworld_track.h"

namespace GridWorld
{

GridWorldTrack::GridWorldTrack(std::string trackStr, GridWorldModel gridworld, system_clock::time_point startTime)
  : trackString{trackStr},
    gridworld{gridworld},
    startTime{startTime}
{
    if (isValidTrackString(trackStr))
    {
        constructTrackPoints();
    }
    else
    {
        throw std::invalid_argument("Invalid point sequence, cannot construct Track.");
    }
}

void GridWorldTrack::constructTrackPoints()
{
    std::istringstream tss(trackString);

    system_clock::time_point currentTime = startTime;
    GridWorldModel::Point point;

    while (tss >> point)
    {
        trackPoints.push_back( {gridworld[point], GridWorldModel::pointToString(point), time_pointTotm(currentTime)} );

        unsigned int numberOfSeconds;
        tss >> numberOfSeconds;
        currentTime += std::chrono::seconds(numberOfSeconds);
        // If there are no trailing time digits then this final input operation fails,
        // but that's not a problem as then the loop terminates anyway.
    }
}

std::string GridWorldTrack::toGPX() const
{
    XML::Generator gpx;

    gpx.basicXMLDeclaration();
    gpx.openBasicGPXElement();

    gpx.openElement("trk",{});

    for (const TrackPoint& trackPoint : trackPoints)
    {
        XML::Attributes attribs =
          { {"lat", std::to_string(trackPoint.position.latitude())},
            {"lon", std::to_string(trackPoint.position.longitude())}
          };

        gpx.openElement("trkpt", attribs);
        gpx.element("ele",{},std::to_string(trackPoint.position.elevation()));
        gpx.element("time",{},dateTimeToString(trackPoint.dateTime));
        gpx.element("name",{},trackPoint.name);
        gpx.closeElement(); // "trkpt"
    }

    return gpx.closeAllElementsAndExtractString();
}

std::string GridWorldTrack::toString() const
{
    return trackString;
}

std::vector<TrackPoint> GridWorldTrack::toTrackPoints() const
{
    return trackPoints;
}

bool GridWorldTrack::isValidTrackString(std::string trackStr)
{
    bool validChars = std::all_of(trackStr.begin(), trackStr.end(),
                                  [] (char c) {return std::isupper(c) || std::isdigit(c);});
    bool noLeadingOrTrailingTime = trackStr.length() == 0 || (isupper(trackStr.front()) && isupper(trackStr.back()));
    return validChars && noLeadingOrTrailingTime && GridWorldRoute::isValidRouteString(routeStringFromTrackString(trackStr));
}

std::string GridWorldTrack::routeStringFromTrackString(std::string trackStr)
{
    std::string routeStr = trackStr;
    routeStr.erase(std::remove_if(routeStr.begin(),routeStr.end(),[] (char c) {return std::isdigit(c);}),
                   routeStr.end());
    return routeStr;
}

std::tm GridWorldTrack::time_pointTotm(system_clock::time_point tp)
{
    std::time_t calendarTime = system_clock::to_time_t(tp);
    return *localtime(&calendarTime);
}

std::string GridWorldTrack::dateTimeToString(const tm& dateTime)
{
    // For documentation on the format specifiers, see: https://en.cppreference.com/w/cpp/io/manip/put_time
    const std::string dateTimeFormat = "%Y-%m-%dT%H:%M:%SZ";

    std::ostringstream dateTimeStream;
    dateTimeStream << std::put_time(&dateTime, dateTimeFormat.c_str());

    return dateTimeStream.str();
}

}
