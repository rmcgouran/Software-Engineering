#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <iterator>

#include "geometry.h"
#include "route.h"

using namespace GPS;

Route::Route(std::vector<RoutePoint> routePointsInput)
{
    if (routePointsInput.empty())
    {
        throw std::invalid_argument("Invalid vector of RoutePoints - Routes must contain at least one point.");
    }
    routePoints.insert(routePoints.end(), routePointsInput.begin(), routePointsInput.end());
}

unsigned int Route::numPoints() const
{
    return routePoints.size();
}

metres Route::totalLength() const
{
    assert(! routePoints.empty());

    metres lengthSoFar = 0.0;

    std::list<RoutePoint>::const_iterator currentPoint = routePoints.begin();
    std::list<RoutePoint>::const_iterator nextPoint = std::next(currentPoint);
    for ( ; nextPoint != routePoints.end(); ++currentPoint, ++nextPoint)
    {
        metres deltaH = Position::horizontalDistanceBetween(currentPoint->position,nextPoint->position);
        metres deltaV = nextPoint->position.elevation() - currentPoint->position.elevation();
        lengthSoFar += pythagoras(deltaH,deltaV);
    }

    return lengthSoFar;
}

metres Route::netLength() const
{
    assert(! routePoints.empty());

    const Position & start  = routePoints.front().position;
    const Position & finish = routePoints.back().position;

    metres deltaH = Position::horizontalDistanceBetween(start,finish);
    metres deltaV = start.elevation() - finish.elevation();
    return pythagoras(deltaH,deltaV);
}

metres Route::totalHeightGain() const
{
    assert(! routePoints.empty());

    metres total = 0.0;

    for (std::list<RoutePoint>::const_iterator current = routePoints.begin(),
                                               next = std::next(current);
         next != routePoints.end();
         ++current, ++next)
    {
        metres deltaV = next->position.elevation() - current->position.elevation();
        if (deltaV > 0.0) total += deltaV; // ignore negative height differences
    }

    return total;
}

metres Route::netHeightGain() const
{
    assert(! routePoints.empty());

    metres deltaV = routePoints.back().position.elevation() - routePoints.front().position.elevation();
    return std::max(deltaV,0.0); // ignore negative height differences
}

degrees Route::maxGradient() const
{
    assert(! routePoints.empty());

    if (routePoints.size() == 1) throw std::domain_error("Cannot compute gradients on a single-point route.");

    degrees maxGrad = -halfRotation/2; // minimum possible gradient value

    for (std::list<RoutePoint>::const_iterator current = routePoints.begin(),
                                               next = std::next(current);
         next != routePoints.end();
         ++current, ++next)
    {
        metres deltaH = Position::horizontalDistanceBetween(current->position,next->position);
        metres deltaV = next->position.elevation() - current->position.elevation();
        degrees grad = radToDeg(std::atan(deltaV/deltaH));
        maxGrad = std::max(maxGrad,grad);
    }

    return maxGrad;
}

degrees Route::minGradient() const
{
    assert(! routePoints.empty());

    if (routePoints.size() == 1) throw std::domain_error("Cannot compute gradients on a single-point route.");

    degrees minGrad = halfRotation/2; // maximum possible gradient value

    for (std::list<RoutePoint>::const_iterator current = routePoints.begin(),
                                               next = std::next(current);
         next != routePoints.end();
         ++current, ++next)
    {
        metres deltaH = Position::horizontalDistanceBetween(current->position,next->position);
        metres deltaV = next->position.elevation() - current->position.elevation();
        degrees grad = radToDeg(std::atan(deltaV/deltaH));
        minGrad = std::min(minGrad,grad);
    }

    return minGrad;
}

degrees Route::steepestGradient() const
{
    assert(! routePoints.empty());

    if (routePoints.size() == 1) throw std::domain_error("Cannot compute gradients on a single-point route.");

    degrees steepestGrad = 0; // minimum possible gradient value

    for (std::list<RoutePoint>::const_iterator current = routePoints.begin(),
                                               next = std::next(current);
         next != routePoints.end();
         ++current, ++next)
    {
        metres deltaH = Position::horizontalDistanceBetween(current->position,next->position);
        metres deltaV = next->position.elevation() - current->position.elevation();
        degrees grad = radToDeg(std::atan(deltaV/deltaH));
        if (std::abs(grad) > std::abs(steepestGrad))
        {
            steepestGrad = grad;
        }
    }
    return steepestGrad;
}

RoutePoint Route::highestPoint() const
{
    assert(! routePoints.empty());

    RoutePoint highestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.elevation() > highestPointSoFar.position.elevation())
        {
            highestPointSoFar = currentPoint;
        }
    }
    return highestPointSoFar;
}

RoutePoint Route::lowestPoint() const
{
    assert(! routePoints.empty());

    RoutePoint lowestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.elevation() < lowestPointSoFar.position.elevation())
        {
            lowestPointSoFar = currentPoint;
        }
    }
    return lowestPointSoFar;
}

RoutePoint Route::mostNorthelyPoint() const
{
    assert(! routePoints.empty());

    RoutePoint northmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.latitude() > northmostPointSoFar.position.latitude())
        {
            northmostPointSoFar = currentPoint;
        }
    }
    return northmostPointSoFar;
}

RoutePoint Route::mostSoutherlyPoint() const
{
    assert(! routePoints.empty());

    RoutePoint southmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.latitude() < southmostPointSoFar.position.latitude())
        {
            southmostPointSoFar = currentPoint;
        }
    }
    return southmostPointSoFar;
}

RoutePoint Route::mostEasterlyPoint() const
{
    assert(! routePoints.empty());

    RoutePoint eastmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.longitude() > eastmostPointSoFar.position.longitude())
        {
            eastmostPointSoFar = currentPoint;
        }
    }
    return eastmostPointSoFar;
}

RoutePoint Route::mostWesterlyPoint() const
{
    assert(! routePoints.empty());

    RoutePoint westmostPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (currentPoint.position.longitude() < westmostPointSoFar.position.longitude())
        {
            westmostPointSoFar = currentPoint;
        }
    }
    return westmostPointSoFar;
}

RoutePoint Route::mostEquatorialPoint() const
{
    assert(! routePoints.empty());

    RoutePoint nearestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (std::abs(currentPoint.position.latitude()) < std::abs(nearestPointSoFar.position.latitude()))
        {
            nearestPointSoFar = currentPoint;
        }
    }
    return nearestPointSoFar;
}

RoutePoint Route::leastEquatorialPoint() const
{
    assert(! routePoints.empty());

    RoutePoint farthestPointSoFar = routePoints.front();
    for (const RoutePoint& currentPoint : routePoints)
    {
        if (std::abs(currentPoint.position.latitude()) > std::abs(farthestPointSoFar.position.latitude()))
        {
            farthestPointSoFar = currentPoint;
        }
    }
    return farthestPointSoFar;
}

Position Route::findPosition(std::string soughtName) const
{
    if (soughtName.empty()) throw std::invalid_argument("Cannot find the position of an empty name.");

    for (const RoutePoint& routePoint : routePoints)
    {
        if (routePoint.name == soughtName) return routePoint.position;
    }

    throw std::domain_error("No position with that name found in the route.");
}

unsigned int Route::timesVisited(std::string soughtName) const
{
    if (soughtName.empty()) throw std::invalid_argument("Cannot find the position of an empty name.");

    unsigned int numberOfVisits = 0;

    for (const RoutePoint& routePoint : routePoints)
    {
        if (routePoint.name == soughtName) ++numberOfVisits;
    }

    return numberOfVisits;
}

RoutePoint Route::operator[](unsigned int index) const
{
    if (index >= routePoints.size())
    {
        throw std::out_of_range("Position index out-of-range.");
    }

    return *std::next(routePoints.begin(),index);
}

RoutePoint Route::nearestPointTo(Position targetPosition) const
{
    assert(! routePoints.empty());

    RoutePoint nearestPointSoFar = routePoints.front();
    metres shortestDistanceSoFar = Position::horizontalDistanceBetween(nearestPointSoFar.position, targetPosition);
    for (const RoutePoint& currentPoint : routePoints)
    {
        metres currentDistance = Position::horizontalDistanceBetween(currentPoint.position, targetPosition);
        if (currentDistance < shortestDistanceSoFar)
        {
            nearestPointSoFar = currentPoint;
            shortestDistanceSoFar = currentDistance;
        }
    }
    return nearestPointSoFar;
}

RoutePoint Route::farthestPointFrom(Position avoidedPosition) const
{
    assert(! routePoints.empty());

    RoutePoint farthestPointSoFar = routePoints.front();
    metres longestDistanceSoFar = Position::horizontalDistanceBetween(farthestPointSoFar.position, avoidedPosition);
    for (const RoutePoint& currentPoint : routePoints)
    {
        metres currentDistance = Position::horizontalDistanceBetween(currentPoint.position, avoidedPosition);
        if (currentDistance > longestDistanceSoFar)
        {
            farthestPointSoFar = currentPoint;
            longestDistanceSoFar = currentDistance;
        }
    }
    return farthestPointSoFar;
}
