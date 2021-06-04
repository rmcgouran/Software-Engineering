#include <algorithm>
#include <stdexcept>

#include "xml/element.h"
#include "xml/generator.h"
#include "gridworld_model.h"
#include "gridworld_route.h"

namespace GridWorld
{

GridWorldRoute::GridWorldRoute(std::string routeStr, GridWorldModel gridworld)
  : routeString{routeStr},
    gridworld{gridworld}
{
    if (isValidRouteString(routeStr))
    {
        constructRoutePoints();
    }
    else
    {
        throw std::invalid_argument("Invalid point sequence, cannot construct Route.");
    }
}

void GridWorldRoute::constructRoutePoints()
{
    for (GridWorldModel::Point point : routeString)
    {
        routePoints.push_back( {gridworld[point], GridWorldModel::pointToString(point)} );
    }
}

std::string GridWorldRoute::toGPX() const
{
    XML::Generator gpx;

    gpx.basicXMLDeclaration();
    gpx.openBasicGPXElement();

    gpx.openElement("rte",{});

    for (const RoutePoint& routePoint : routePoints)
    {
        XML::Attributes attribs =
          { {"lat", std::to_string(routePoint.position.latitude())},
            {"lon", std::to_string(routePoint.position.longitude())}
          };

        gpx.openElement("rtept",attribs);
        gpx.element("name",{},routePoint.name);
        gpx.element("ele",{},std::to_string(routePoint.position.elevation()));
        gpx.closeElement(); // "rtept"
    }

    return gpx.closeAllElementsAndExtractString();
}

std::string GridWorldRoute::toNMEA() const
{
    // Stub definition
    return "";
}

std::string GridWorldRoute::toString() const
{
    return routeString;
}

std::vector<RoutePoint> GridWorldRoute::toRoutePoints() const
{
    return routePoints;
}

bool GridWorldRoute::isValidRouteString(std::string routeStr)
{
    // To be valid, all chars must be in the range 'A'..'Y'.
    return std::all_of(routeStr.begin(), routeStr.end(),
                       [] (char point)->bool {return point >= 'A' && point <= 'Y';});
}

}
