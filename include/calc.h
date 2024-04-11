#include <math.h>
#define EARTH_RADIUS 6371.0

double to_radians(double degrees);

double vincenty_distance(double lat1, double lon1, double lat2, double lon2);