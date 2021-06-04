#include <iostream>
#include <fstream>

#include "logs.h"

#include "gridworld_model.h"
#include "gridworld_route.h"
#include "gridworld_track.h"

using namespace GPS;
using namespace GridWorld;

using std::cout;
using std::endl;

int main()
{
   /////////////////////////////////////////////////////////////

   /* Basic Usage Examples - Using default GridWorld settings */

   /* A simple route using the default GridWorld model. */
   //cout << GridWorldRoute("AGM").toGPX() << endl;

   /* // A simple track using the default GridWorld model. */
   // cout << GridWorldTrack("A1G3M").toGPX() << endl;

   /////////////////////////////////////////////////////////////

   /* Advanced Usage Examples - Configuring GridWorld settings  */
    //cout << "Test" << endl;
    const Position & posM = Position(50,30,2000); // Position of the central point 'M'.
    metres horizontalGridUnit = 10000; // Horizontal distance between grid points.
    metres verticalGridUnit = 1000; // Vertical distance between grid levels.

    GridWorldModel gwModel {posM, horizontalGridUnit, verticalGridUnit};

    /* A simple route using the specified GridWorld model. */
    // cout << GridWorldRoute("AGM",gwModel).toGPX() << endl;

    /* A simple track using the specified GridWorld model. */
     cout << GridWorldRoute("ABCD",gwModel).toGPX() << endl;

    /////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////
}
