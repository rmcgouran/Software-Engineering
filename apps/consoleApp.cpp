#include <iostream>
#include <filesystem>

#include "logs.h"
#include "parseGPX.h"
#include "route.h"

using std::cout;
using std::endl;

int main()
{
    std::string filepath = GPS::LogFiles::GPXRoutesDir + "/NorthYorkMoors.gpx";

    if (! std::filesystem::exists(filepath))
    {
        cout << "Could not open log file: " + filepath << endl;
        cout << "(If you're running at the command-line, you need to 'cd' into the 'bin/' directory first.)";
    }

    bool isFileName = true;
    GPS::Route exampleRoute { GPX::parseRoute(filepath, isFileName) };

    cout << "Number of points: " << exampleRoute.numPoints()     << endl;
    cout << "Net length: "       << exampleRoute.netLength()     << endl;
    cout << "Total length: "     << exampleRoute.totalLength()   << endl;
    cout << "Net height gain: "  << exampleRoute.netHeightGain() << endl;
    // etc.
}
