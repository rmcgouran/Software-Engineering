#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "types.h"
#include "points.h"
#include "route.h"
#include "track.h"
#include "gridworld_route.h"
#include "gridworld_track.h"

using namespace GPS;
using namespace GridWorld;

/* For Route.operator[], there are two main aspects to test:
 *   - the length of the Route;
 *   - the index number used.
 *
 * The actual data stored in the routes (Position and name) isn't particularly
 * significant for this function, as the function doesn't process that data at all when
 * location a point by index number. However, we should ensure that each test point is
 * different, as otherwise a bug that returns the wrong point might go undetected if the
 * other point returned is another copy of the point that should have been returned.
 *
 * We should test both valid and invalid indices. A valid index is less than the length
 * of the route, and returns the RoutePoint. An invalid index is greater than the length
 * of the route, and we should check that this throws the expected exception.
 *
 * Edges and boundaries:
 *  - For the index number there is one edge case to test: index number 0.
 *  - There is a boundary between valid and invalid indices - we should test the largest
 *    valid index and the smallest invalid index.
 *  - The route length itself has a single edge case: a route containing a single point.
 *    (A route with zero points is prohibited by the Route class constructor.)
 *
 * Note: We do not need to consider negative index numbers because the parameter type is
 * 'unsigned int'. If, in future, this was changed to a (signed) int, then there would
 * be another boundary between 0 and -1, and we would need to test that negative indices
 * throw an exception.
 *
 * Other things to test are that indexing works with the Track sub-class, including:
 *  - that indexing is independent of time information;
 *  - that index numbers are correctly updated after track points are merged because
 *    they are within the 'granularity' distance.
 */

BOOST_AUTO_TEST_SUITE( Route_Indexing )

const double epsilon = 0.0001;

const Position posM = Position(0,0,0);
const metres horizontalGridUnit = 100000;
const metres verticalGridUnit = 0;
const GridWorldModel gwOnEquator {posM,horizontalGridUnit,verticalGridUnit};


// Typical input - indexing in the middle of a route containing multiple points.
BOOST_AUTO_TEST_CASE( MiddleOfRoute )
{
    const Route route {GridWorldRoute("KLMNO",gwOnEquator).toRoutePoints()};
    const unsigned int index = 2;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Edge case - the first point in the route.
BOOST_AUTO_TEST_CASE( StartOfRoute )
{
    const Route route {GridWorldRoute("MIDC",gwOnEquator).toRoutePoints()};
    const unsigned int index = 0;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Boundary case - the last valid index in the route.
BOOST_AUTO_TEST_CASE( EndOfRoute )
{
    const Route route {GridWorldRoute("AYXM",gwOnEquator).toRoutePoints()};
    const unsigned int index = 3;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Edge case - a route containing a single point.
BOOST_AUTO_TEST_CASE( OnePointRoute )
{
    const Route route {GridWorldRoute("M",gwOnEquator).toRoutePoints()};
    const unsigned int index = 0;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Invalid input - index out of range.
BOOST_AUTO_TEST_CASE( OutOfRange )
{
    const Route route {GridWorldRoute("ABCDE",gwOnEquator).toRoutePoints()};
    const unsigned int index = 20;

    BOOST_CHECK_THROW( route[index], std::out_of_range );
}

// Boundary case - first index that is out-of-range.
BOOST_AUTO_TEST_CASE( OutOfRangeByOne )
{
    const Route route {GridWorldRoute("ABCDE",gwOnEquator).toRoutePoints()};
    const unsigned int index = 5;

    BOOST_CHECK_THROW( route[index], std::out_of_range );
}

// A test when the result isn't Point M (just to ensure the function doesn't always return M).
BOOST_AUTO_TEST_CASE( ResultOtherThanM )
{
    const Position pos0 = Position(20,85);
    const Position pos1 = Position(30,65);
    const Position pos2 = Position(40,45);

    const std::vector<RoutePoint> routePoints = { { pos0, "P0" },  { pos1, "P1" }, { pos2, "P2" } };
    const Route route {routePoints};

    const unsigned int index = 1;
    const std::string expectedName = "P1";
    const Position expectedPosition = pos1;

    RoutePoint actualPoint = route[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Check that indexing also works correctly for Tracks.
BOOST_AUTO_TEST_CASE( TrackPoints )
{
    const std::vector<TrackPoint> trackPoints = GridWorldTrack("A1G1M1S1Y",gwOnEquator).toTrackPoints();
    const metres granularity = horizontalGridUnit * 0.1; // Ensure no points are merged.
    const Track track {trackPoints,granularity};

    const unsigned int index = 2;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = track[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Check that indexing isn't based on time values by having varied time durations.
BOOST_AUTO_TEST_CASE( TrackWithVaryingTimeDurations )
{
    const std::vector<TrackPoint> trackPoints = GridWorldTrack("A1B3C7M2X99Y",gwOnEquator).toTrackPoints();
    const metres granularity = horizontalGridUnit * 0.1; // Ensure no points are merged.
    const Track track {trackPoints,granularity};

    const unsigned int index = 3;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = track[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

// Check that index numbers are correctly updated after points are merged.
BOOST_AUTO_TEST_CASE( TrackAfterMerging )
{
    const std::vector<TrackPoint> trackPoints = GridWorldTrack("A1B1C1D1M1X1Y",gwOnEquator).toTrackPoints();
    const metres granularity = horizontalGridUnit * 1.5; // B, D and Y are merged.
    const Track track {trackPoints,granularity};

    const unsigned int index = 2;
    const std::string expectedName = "M";
    const Position expectedPosition = posM;

    RoutePoint actualPoint = track[index];

    BOOST_CHECK_EQUAL( actualPoint.name, expectedName );
    BOOST_CHECK_SMALL( Position::horizontalDistanceBetween(actualPoint.position,expectedPosition), epsilon );
}

BOOST_AUTO_TEST_SUITE_END()
