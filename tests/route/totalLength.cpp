#include <boost/test/unit_test.hpp>

#include "types.h"
#include "points.h"
#include "route.h"
#include "track.h"
#include "parseGPX.h"


namespace utf = boost::unit_test;
using namespace GPS;

BOOST_AUTO_TEST_SUITE( Route_totalLength )

const double tol = 0.0001;

// Checking a small route size - hundreds of metres.
BOOST_AUTO_TEST_CASE( Extreme_Input_Three_Digit_Number,  * utf::tolerance(tol) )
{
     metres expectedResult = 111.195;

     const std::string gpxData = "<gpx><rte><name>AB</name><rtept lat=\"0.00089982\" lon=\"109.321\"><name>A</name><ele>-100.000000</ele></rtept><rtept lat=\"0.00089982\" lon=\"109.322\"><name>B</name><ele>-100.000000</ele></rtept></rte></gpx>";
     const bool isFileName = false;
     const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
     Route route = Route{ routePoints };

     BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking an average route size - tens of thousands of metres.
BOOST_AUTO_TEST_CASE( Typical_Input_Five_Digit_Number,  * utf::tolerance(tol) )
{
    metres expectedResult = 30022.5;

    const std::string gpxData = "<gpx><rte><name>ABCB</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.322\"><name>C</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a long route - hundreds of thousands of metres.
BOOST_AUTO_TEST_CASE( Typical_Input_Six_Digit_Number, * utf::tolerance(tol) )
{
    metres expectedResult = 121525;

    const std::string gpxData = "<gpx><rte><name>AGM</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.681\"><name>G</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.22\"><name>M</name><ele>0.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a very long route - millions of metres.
BOOST_AUTO_TEST_CASE( Typical_Input_Eight_Digit_Number, * utf::tolerance(tol) )
{
    metres expectedResult = 2233405.18;

    const std::string gpxData = "<gpx><rte><name>ABCDEJOTYXWVUPKFGHINSRQLM</name><rtept lat=\"1.61968\" lon=\"107.705\"><name>A</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"108.514\"><name>B</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"109.322\"><name>C</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"110.131\"><name>D</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"110.939\"><name>E</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"110.939\"><name>J</name><ele>-180000.000000</ele></rtept><rtept lat=\"0\" lon=\"110.939\"><name>O</name><ele>-180000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"110.939\"><name>T</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"110.939\"><name>Y</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"110.131\"><name>X</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"109.322\"><name>W</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"108.514\"><name>V</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"107.705\"><name>U</name><ele>-180000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"107.705\"><name>P</name><ele>-180000.000000</ele></rtept><rtept lat=\"0\" lon=\"107.705\"><name>K</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"107.705\"><name>F</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"108.514\"><name>G</name><ele>-90000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"109.322\"><name>H</name><ele>-90000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"110.131\"><name>I</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"110.131\"><name>N</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"110.131\"><name>S</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"109.322\"><name>R</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"108.514\"><name>Q</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"108.514\"><name>L</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"109.322\"><name>M</name><ele>0.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

const std::string gpxData = "";
const bool isFileName = false;
const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
Route route = Route{ routePoints };

// Checking the largest size of a route - hundreds of billions of metres.
BOOST_AUTO_TEST_CASE(cExtreme_Input_Twelve_Digit_Number, * utf::tolerance(tol) )
{
    metres expectedResult = 400045320843;

    const std::string gpxData = "<gpx><rte><name>ABGLKF</name><rtept lat=\"0.143971\" lon=\"21.0357\"><name>A</name><ele>-400000000000.000000</ele></rtept><rtept lat=\"0.143971\" lon=\"-114.821\"><name>B</name><ele>-400000000000.000000</ele></rtept><rtept lat=\"0.0719856\" lon=\"109.322\"><name>G</name><ele>-200000000000.000000</ele></rtept><rtept lat=\"0\" lon=\"-26.5346\"><name>L</name><ele>-200000000000.000000</ele></rtept><rtept lat=\"0\" lon=\"109.322\"><name>K</name><ele>-400000000000.000000</ele></rtept><rtept lat=\"0.0719856\" lon=\"-114.821\"><name>F</name><ele>-400000000000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route that is a straight line with no slope.
BOOST_AUTO_TEST_CASE( Route_Is_Straight_Line, * utf::tolerance(tol) )
{
    metres expectedResult= 40030;

    const std::string gpxData = "<gpx><rte><name>ABCDE</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.322\"><name>C</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.412\"><name>D</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.502\"><name>E</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);

}

// Checking a route that changes direction once (e.g East to South).
BOOST_AUTO_TEST_CASE( Route_Changes_Direction_Once,* utf::tolerance(tol) )
{
    metres expectedResult= 239735;

    const std::string gpxData = "<gpx><rte><name>ABCDEJOTY</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.322\"><name>C</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.412\"><name>D</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.502\"><name>E</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.951\"><name>J</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.4\"><name>O</name> <ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.849\"><name>T</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"111.298\"><name>Y</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a straight line route with a downward slope.
BOOST_AUTO_TEST_CASE( Route_Downward_Slope,* utf::tolerance(tol) )
{
    metres expectedResult = 121525;

    const std::string gpxData = "<gpx><rte><name>MSY</name><rtept lat=\"0.179964\" lon=\"110.22\"><name>M</name><ele>0.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.759\"><name>S</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"111.298\"><name>Y</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a straight line route with an upward slope.
BOOST_AUTO_TEST_CASE( Route_Upward_Slope,* utf::tolerance(tol) )
{
    metres expectedResult = 121525;

    const std::string gpxData = "<gpx><rte><name>AGM</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.681\"><name>G</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.22\"><name>M</name><ele>0.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route that loops back to the point it began with.
BOOST_AUTO_TEST_CASE( Route_Looped,* utf::tolerance(tol) )
{
    metres expectedResult = 239735;

    const std::string gpxData = "<gpx><rte><name>GHINSRQLG</name><rtept lat=\"0.179964\" lon=\"109.681\"><name>G</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.771\"><name>H</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.861\"><name>I</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.31\"><name>N</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.759\"><name>S</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.67\"><name>R</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.58\"><name>Q</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.131\"><name>L</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.681\"><name>G</name><ele>-10000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route that spirals.
BOOST_AUTO_TEST_CASE( Route_Spiral,* utf::tolerance(tol) )
{
    metres expectedResult = 2233405.18;

    const std::string gpxData = "<gpx><rte><name>ABCDEJOTYXWVUPKFGHINSRQLM</name><rtept lat=\"1.61968\" lon=\"107.705\"><name>A</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"108.514\"><name>B</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"109.322\"><name>C</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"110.131\"><name>D</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"110.939\"><name>E</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"110.939\"><name>J</name><ele>-180000.000000</ele></rtept><rtept lat=\"0\" lon=\"110.939\"><name>O</name><ele>-180000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"110.939\"><name>T</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"110.939\"><name>Y</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"110.131\"><name>X</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"109.322\"><name>W</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"108.514\"><name>V</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"107.705\"><name>U</name><ele>-180000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"107.705\"><name>P</name><ele>-180000.000000</ele></rtept><rtept lat=\"0\" lon=\"107.705\"><name>K</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"107.705\"><name>F</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"108.514\"><name>G</name><ele>-90000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"109.322\"><name>H</name><ele>-90000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"110.131\"><name>I</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"110.131\"><name>N</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"110.131\"><name>S</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"109.322\"><name>R</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"108.514\"><name>Q</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"108.514\"><name>L</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"109.322\"><name>M</name><ele>0.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

/* checking a path/route shape which ends up changing direction so that the path goes back onitself.
* specifically, distance shouldn't be subtracted as the path goes back on itself.
*/
BOOST_AUTO_TEST_CASE( Route_Goes_Back_On_Itself,* utf::tolerance(tol) )
{
    metres expectedResult = 30022.5;

    const std::string gpxData = "<gpx><rte><name>ABCB</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.322\"><name>C</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route which travels in every direction (North, East, South and West).
BOOST_AUTO_TEST_CASE( Route_Goes_Every_Direction, * utf::tolerance(tol) )
{
    metres expectedResult = 400045320843.68;

    const std::string gpxData = "<gpx><rte><name>ABGLKF</name><rtept lat=\"0.143971\" lon=\"21.0357\"><name>A</name><ele>-400000000000.000000</ele></rtept><rtept lat=\"0.143971\" lon=\"-114.821\"><name>B</name><ele>-400000000000.000000</ele></rtept><rtept lat=\"0.0719856\" lon=\"109.322\"><name>G</name><ele>-200000000000.000000</ele></rtept><rtept lat=\"0\" lon=\"-26.5346\"><name>L</name><ele>-200000000000.000000</ele></rtept><rtept lat=\"0\" lon=\"109.322\"><name>K</name><ele>-400000000000.000000</ele></rtept><rtept lat=\"0.0719856\" lon=\"-114.821\"><name>F</name><ele>-400000000000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route with a single point - 0 distance should be travelled.
BOOST_AUTO_TEST_CASE( Route_Has_Single_Point )
{
    const std::string gpxData = "<gpx><rte><name>Q</name><rtept lat=\"-0.89982\" lon=\"-0.898312\"><name>Q</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_CHECK_SMALL(route.totalLength(), tol);
}

// Checking a route with two points.
BOOST_AUTO_TEST_CASE( Route_Has_Two_Points, * utf::tolerance(tol) )
{
     metres expectedResult = 111.195;

     const std::string gpxData = "<gpx><rte><name>AB</name><rtept lat=\"0.00089982\" lon=\"109.321\"><name>A</name><ele>-100.000000</ele></rtept><rtept lat=\"0.00089982\" lon=\"109.322\"><name>B</name><ele>-100.000000</ele></rtept></rte></gpx>";
     const bool isFileName = false;
     const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
     Route route = Route{ routePoints };

     BOOST_TEST(route.totalLength() == expectedResult);

}

// Checking a route with three points.
BOOST_AUTO_TEST_CASE( Route_Has_Three_Points,* utf::tolerance(tol) )
{
    metres expectedResult = 121525;

    const std::string gpxData = "<gpx><rte><name>AGM</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.681\"><name>G</name><ele>-10000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.22\"><name>M</name><ele>0.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route with four points.
BOOST_AUTO_TEST_CASE( Route_Has_Four_Points, * utf::tolerance(tol) )
{
    metres expectedResult = 30022.5;

    const std::string gpxData = "<gpx><rte><name>ABCB</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.322\"><name>C</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}

// Checking a route with nine points.
BOOST_AUTO_TEST_CASE( Route_Has_Nine_Points, * utf::tolerance(tol) )
{
     metres expectedResult= 239735;

     const std::string gpxData = "<gpx><rte><name>ABCDEJOTY</name><rtept lat=\"0.179964\" lon=\"109.142\"><name>A</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.232\"><name>B</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.322\"><name>C</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.412\"><name>D</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.502\"><name>E</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"109.951\"><name>J</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.4\"><name>O</name> <ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"110.849\"><name>T</name><ele>-20000.000000</ele></rtept><rtept lat=\"0.179964\" lon=\"111.298\"><name>Y</name><ele>-20000.000000</ele></rtept></rte></gpx>";
     const bool isFileName = false;
     const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
     Route route = Route{ routePoints };

     BOOST_TEST(route.totalLength() == expectedResult);
}

//checking a route that uses every single point available.
BOOST_AUTO_TEST_CASE( Route_Has_Every_Point, * utf::tolerance(tol) )
{
    metres expectedResult = 2233405.18;

    const std::string gpxData = "<gpx><rte><name>ABCDEJOTYXWVUPKFGHINSRQLM</name><rtept lat=\"1.61968\" lon=\"107.705\"><name>A</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"108.514\"><name>B</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"109.322\"><name>C</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"110.131\"><name>D</name><ele>-180000.000000</ele></rtept><rtept lat=\"1.61968\" lon=\"110.939\"><name>E</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"110.939\"><name>J</name><ele>-180000.000000</ele></rtept><rtept lat=\"0\" lon=\"110.939\"><name>O</name><ele>-180000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"110.939\"><name>T</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"110.939\"><name>Y</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"110.131\"><name>X</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"109.322\"><name>W</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"108.514\"><name>V</name><ele>-180000.000000</ele></rtept><rtept lat=\"-1.61968\" lon=\"107.705\"><name>U</name><ele>-180000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"107.705\"><name>P</name><ele>-180000.000000</ele></rtept><rtept lat=\"0\" lon=\"107.705\"><name>K</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"107.705\"><name>F</name><ele>-180000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"108.514\"><name>G</name><ele>-90000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"109.322\"><name>H</name><ele>-90000.000000</ele></rtept><rtept lat=\"0.809838\" lon=\"110.131\"><name>I</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"110.131\"><name>N</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"110.131\"><name>S</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"109.322\"><name>R</name><ele>-90000.000000</ele></rtept><rtept lat=\"-0.809838\" lon=\"108.514\"><name>Q</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"108.514\"><name>L</name><ele>-90000.000000</ele></rtept><rtept lat=\"0\" lon=\"109.322\"><name>M</name><ele>0.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_TEST(route.totalLength() == expectedResult);
}


BOOST_AUTO_TEST_CASE(Route_Has_Two_Points_Same_Position)   //checking that a route with the 2 points which are the same returns 0, as no distance would be travelled.
{
    const std::string gpxData = "<gpx><rte><name>AA</name><rtept lat=\"0.000179964\" lon=\"109.322\"><name>A</name><ele>-20.000000</ele></rtept><rtept lat=\"0.000179964\" lon=\"109.322\"><name>A</name><ele>-20.000000</ele></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_CHECK_SMALL(route.totalLength(), tol);
}

//check that an empty route, no points, doesn't throw an error and is equal to 0 (As no distance is travelled).
BOOST_AUTO_TEST_CASE(No_Route)
{
    const std::string gpxData = "<gpx><rte><name></name><rtept lat=\"0\" lon=\"0\"></rtept></rte></gpx>";
    const bool isFileName = false;
    const std::vector<RoutePoint> routePoints = GPX::parseRoute(gpxData, isFileName);
    Route route = Route{ routePoints };

    BOOST_CHECK_NO_THROW(route.totalLength());
    BOOST_CHECK_SMALL(route.totalLength(), tol);
}

BOOST_AUTO_TEST_SUITE_END()
