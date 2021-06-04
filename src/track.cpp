#include <cassert>
#include <cmath>
#include <stdexcept>

#include "geometry.h"
#include "track.h"

using namespace GPS;

using std::chrono::seconds;
using std::chrono::duration_cast;

Track::Track(std::vector<TrackPoint> trackPoints, metres granularity)
{
    for (const TrackPoint& trackPoint : trackPoints)
    {
        routePoints.push_back({trackPoint.position,trackPoint.name});
        timeStamps.push_back(tmToTimeStamp(trackPoint.dateTime));
    }

    setGranularity(granularity);
}

void Track::setGranularity(metres newGranularity)
{
    assert( ! routePoints.empty());
    assert( routePoints.size() == timeStamps.size() );

    metres oldGranularity = granularity;
    granularity = newGranularity;

    if (newGranularity > oldGranularity)
    {
        std::list<RoutePoint>::iterator currentPoint = routePoints.begin();
        std::list<RoutePoint>::iterator nextPoint = std::next(currentPoint);
        std::list<TimeStamp>::iterator currentTimeStamp = timeStamps.begin();
        std::list<TimeStamp>::iterator nextTimeStamp = std::next(currentTimeStamp);
        for (; nextPoint != routePoints.end(); ++currentPoint, ++nextPoint, ++currentTimeStamp, ++nextTimeStamp)
        {
            if (areSameLocation(currentPoint->position,nextPoint->position))
            {
                nextPoint = routePoints.erase(nextPoint);
                currentTimeStamp->departure = nextTimeStamp->departure;
                nextTimeStamp = timeStamps.erase(nextTimeStamp);
            }
        }
    }
}

seconds Track::totalTime() const
{
    assert(! timeStamps.empty());
    return duration_cast<seconds>(timeStamps.back().departure - timeStamps.front().arrival);
}

seconds Track::restingTime() const
{
    seconds total = seconds::zero();
    for (const TimeStamp& timeStamp : timeStamps)
    {
        total += duration_cast<seconds>(timeStamp.departure - timeStamp.arrival);
    }
    return total;
}

seconds Track::travellingTime() const
{
    return totalTime() - restingTime();
}

seconds Track::longestRest() const
{
    seconds maxRest = seconds::zero();
    for (const TimeStamp& timeStamp : timeStamps)
    {
        seconds restLength = duration_cast<seconds>(timeStamp.departure - timeStamp.arrival);
        maxRest = std::max(maxRest,restLength);
    }
    return maxRest;
}

speed Track::maxSpeed() const
{
    assert( ! routePoints.empty());
    assert( routePoints.size() == timeStamps.size() );

    speed ms = 0;

    std::list<RoutePoint>::const_iterator currentPoint = routePoints.begin();
    std::list<RoutePoint>::const_iterator nextPoint = std::next(currentPoint);
    std::list<TimeStamp>::const_iterator currentTimeStamp = timeStamps.begin();
    std::list<TimeStamp>::const_iterator nextTimeStamp = std::next(currentTimeStamp);
    for (; nextPoint != routePoints.end(); ++currentPoint, ++nextPoint, ++currentTimeStamp, ++nextTimeStamp)
    {
        seconds time = duration_cast<seconds>(nextTimeStamp->arrival - currentTimeStamp->departure);

        if (time == seconds::zero()) throw std::domain_error("Cannot compute speed over a zero duration.");

        metres deltaH = Position::horizontalDistanceBetween(currentPoint->position,nextPoint->position);
        metres deltaV = nextPoint->position.elevation() - currentPoint->position.elevation();
        metres distance = pythagoras(deltaH,deltaV);
        ms = std::max(ms,distance/time.count());
    }

    return ms;
}

speed Track::averageSpeed(bool includeRests) const
{
    if (routePoints.size() == 1) return 0;
    seconds time = (includeRests ? totalTime() : travellingTime());
    if (time == seconds::zero()) throw std::domain_error("Cannot compute speed over a zero duration.");
    return totalLength() / time.count();
}

speed Track::maxRateOfAscent() const
{
    assert( ! routePoints.empty());
    assert( routePoints.size() == timeStamps.size() );

    speed ms = 0;

    std::list<RoutePoint>::const_iterator currentPoint = routePoints.begin();
    std::list<RoutePoint>::const_iterator nextPoint = std::next(currentPoint);
    std::list<TimeStamp>::const_iterator currentTimeStamp = timeStamps.begin();
    std::list<TimeStamp>::const_iterator nextTimeStamp = std::next(currentTimeStamp);
    for (; nextPoint != routePoints.end(); ++currentPoint, ++nextPoint, ++currentTimeStamp, ++nextTimeStamp)
    {
        seconds time = duration_cast<seconds>(nextTimeStamp->arrival - currentTimeStamp->departure);

        if (time == seconds::zero()) throw std::domain_error("Cannot compute speed over a zero duration.");

        metres height = nextPoint->position.elevation() - currentPoint->position.elevation();
        ms = std::max(ms,height/time.count());
    }

    return ms;
}

speed Track::maxRateOfDescent() const
{
    assert( ! routePoints.empty());
    assert( routePoints.size() == timeStamps.size() );

    speed ms = 0;

    std::list<RoutePoint>::const_iterator currentPoint = routePoints.begin();
    std::list<RoutePoint>::const_iterator nextPoint = std::next(currentPoint);
    std::list<TimeStamp>::const_iterator currentTimeStamp = timeStamps.begin();
    std::list<TimeStamp>::const_iterator nextTimeStamp = std::next(currentTimeStamp);
    for (; nextPoint != routePoints.end(); ++currentPoint, ++nextPoint, ++currentTimeStamp, ++nextTimeStamp)
    {
        seconds time = duration_cast<seconds>(nextTimeStamp->arrival - currentTimeStamp->departure);

        if (time == seconds::zero()) throw std::domain_error("Cannot compute speed over a zero duration.");

        metres height = currentPoint->position.elevation() - nextPoint->position.elevation();
        ms = std::max(ms,height/time.count());
    }

    return ms;
}

std::string Track::findNameOf(Position soughtPosition) const
{
    for (const RoutePoint& routePoint : routePoints)
    {
        if (areSameLocation(routePoint.position, soughtPosition))
        {
            std::string name = routePoint.name;
            return name.empty() ? "Unnamed Position" : name;
        }
    }

    throw std::domain_error("Position not found in route.");
}

unsigned int Track::timesVisited(Position soughtPosition) const
{
    unsigned int numberOfVisits = 0;

    for (const RoutePoint& routePoint : routePoints)
    {
        if (areSameLocation(routePoint.position, soughtPosition)) ++numberOfVisits;
    }

    return numberOfVisits;
}

bool Track::containsCycles() const
{
    for (std::list<RoutePoint>::const_iterator current = routePoints.begin(); current != routePoints.end(); ++current)
    {
        for (std::list<RoutePoint>::const_iterator later = std::next(current); later != routePoints.end(); ++later)
        {
            if (areSameLocation(current->position,later->position)) return true;
        }
    }
    return false;
}

bool Track::areSameLocation(Position p1, Position p2) const
{
    return (Position::horizontalDistanceBetween(p1,p2) < granularity);
}

Track::TimeStamp Track::tmToTimeStamp(std::tm dateTime)
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&dateTime));
    return {tp,tp};
}
