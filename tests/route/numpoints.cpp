#include <boost/test/unit_test.hpp>

#include "types.h"
#include "points.h"
#include "route.h"
#include "track.h"
#include "gridworld_track.h"

using namespace GPS;
using namespace GridWorld;

/* For Route.numPoints() the main test consideration is to test routes with different
 * numbers of route points. The actual data stored in those routes (Position and name)
 * isn't significant for this function, as the function doesn't process that data at
 * all when counting points.
 *
 * There is one major edge case for this function: a route with only a single point
 * (a route with zero points is prohibited by the Route class constructor).
 *
 * There are a few other aspects to test that shouldn't affect the function result,
 * but potentially might trigger a bug.
 *   - Testing a Route with some unnamed route points, just in case names are being
 *     counted rather than points.
 *   - Testing using a Track sub-class.
 *
 * Finally, there is the interaction with the 'granularity' of the Track. If some
 * points are merged, then the function result should then not include any removed
 * points. (A plausible bug would be for the number of points to be counted before
 * processing the merging.)
 */

///////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_SUITE( Route_numPoints )

const Position pos1 = Position(20,2);
const Position pos2 = Position(30,3);
const Position pos3 = Position(40,4);

const RoutePoint rtpt1 = { pos1, "P1" };
const RoutePoint rtpt2 = { pos2, "P2" };
const RoutePoint rtpt3 = { pos3, "P3" };

const metres horizontalGridUnit = 100000;
const metres verticalGridUnit = 0;
GridWorldModel gwNearEquator {Earth::Pontianak,horizontalGridUnit,verticalGridUnit};

// A typical small route with multiple points.
BOOST_AUTO_TEST_CASE( MultiplePoints )
{
    const std::vector<RoutePoint> routePoints = { rtpt1, rtpt2, rtpt3 };
    const Route route {routePoints};
    const int expectedNumPoints = 3;

    int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// Edge case: a route with the fewest possible number of points.
BOOST_AUTO_TEST_CASE( SinglePoint )
{
    const std::vector<RoutePoint> routePoints = { rtpt1 };
    const Route route {routePoints};
    const int expectedNumPoints = 1;

    int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// A route containing unnamed points, to check empty names do not affect the result.
BOOST_AUTO_TEST_CASE( UnnamedPoints )
{
    const std::vector<RoutePoint> routePoints = { {pos1,""}, {pos2,""}, {pos3,""} };
    const Route route {routePoints};
    const int expectedNumPoints = 3;

    int actualNumPoints = route.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// Check that numPoints() also works correctly for Tracks.
BOOST_AUTO_TEST_CASE( TrackPoints )
{
    const std::vector<TrackPoint> trackPoints = GridWorldTrack("A1B1C1D1E",gwNearEquator).toTrackPoints();
    const metres granularity = horizontalGridUnit * 0.1; // Ensure no points are merged.
    const Track track {trackPoints,granularity};
    const int expectedNumPoints = 5;

    int actualNumPoints = track.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

// Check that numPoints() takes into account merged points.
BOOST_AUTO_TEST_CASE( SomeMergedTrackPoints )
{
    const std::vector<TrackPoint> trackPoints = GridWorldTrack("A1B1C1D1E",gwNearEquator).toTrackPoints();
    const metres granularity = horizontalGridUnit * 1.5; // Every second point is merged.
    const Track track {trackPoints,granularity};
    const int expectedNumPoints = 3;

    int actualNumPoints = track.numPoints();

    BOOST_CHECK_EQUAL( actualNumPoints, expectedNumPoints );
}

BOOST_AUTO_TEST_SUITE_END()

///////////////////////////////////////////////////////////////////////////////
