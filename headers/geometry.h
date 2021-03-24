#ifndef GEOMETRY_H_211217
#define GEOMETRY_H_211217

#include "types.h"

namespace GPS
{
  extern const double pi;
  extern const degrees fullRotation;
  extern const degrees halfRotation;
  extern const degrees poleLatitude;
  extern const degrees antiMeridianLongitude;

  // Compute hypotenuse of right-angled triangle in two dimensions.
  double pythagoras(double,double);

  // Compute hypotenuse of right-angled triangle in three dimensions.
  double pythagoras(double,double,double);

  // Convert from degrees to radians.
  radians degToRad(degrees);

  // Convert from radians to degrees.
  radians radToDeg(degrees);

  // Sine squared function: sin^2(x)
  double sinSqr(radians);

  // Ensure degrees are in (-180,180] range.
  degrees normaliseDeg(degrees);
}

#endif
