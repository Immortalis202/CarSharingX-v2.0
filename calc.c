#include "include/calc.h"

// #define PI 3.14159265358979323846

// double_t flattening = 1.0 / 298.257223563;
// double_t radiusEarth = 6378.137;

// double_t to_radians(double_t deg);
// double_t reducedLatitudes(double_t inpLat);
// double_t centreangle(double_t latA, double_t longA, double_t latB, double_t longB);
// double_t bigX(double_t angle, double_t latA, double_t latB);
// double_t bigY(double_t angle, double_t latA, double_t latB);
// double_t distance(double_t angle, double_t X, double_t Y);
// double_t distancePoint(double_t longA, double_t longB, double_t latA, double_t latB );

// double_t reducedLatitudes(double_t inpLat) {
//     inpLat = to_radians(inpLat);
//     double_t out = atan((1 - flattening) * tan(inpLat));
//     return out;
// }

// double_t centreangle(double_t latA, double_t longA, double_t latB, double_t longB) {
//     double_t A = sin(fabs(latA - latB) / 2.0);
//     A = A * A;
//     double_t B = (pow(cos((latA + latB) / 2.0), 2) - pow(sin(fabs(latA - latB) / 2.0), 2)) * pow(sin(fabs(longA - longB) / 2.0), 2);
//     double_t sigmac = 2 * sqrt(A + B);
//     double_t sigma = 2 * asin(sigmac / 2.0);
//     return sigma;
// }

// double_t bigX(double_t angle, double_t latA, double_t latB) {
//     double_t P = (latA + latB) / 2.0;
//     double_t Q = (latB - latA) / 2.0;
//     double_t A = angle - sin(angle);
//     double_t B = (pow(sin(P), 2) * pow(cos(Q), 2)) / pow(cos(angle / 2.0), 2);
//     double_t X = A * B;
//     return X;
// }

// double_t bigY(double_t angle, double_t latA, double_t latB) {
//     double_t P = (latA + latB) / 2.0;
//     double_t Q = (latB - latA) / 2.0;
//     double_t A = angle + sin(angle);
//     double_t B = (pow(cos(P), 2) * pow(sin(Q), 2)) / pow(sin(angle / 2.0), 2);
//     double_t Y = A * B;
//     return Y;
// }

// double_t distance(double_t angle, double_t X, double_t Y) {
//     double_t dist = radiusEarth * (angle - (flattening / 2.0) * (X + Y));
//     return dist;
// }

// double_t distancePoint(double_t longA, double_t longB, double_t latA, double_t latB ) {
//     longA = to_radians(45.4642);
//     //bologna
//     longB = to_radians(44.4949);

//     latA = reducedLatitudes(9.1900);
//     //bologna
//     latB = reducedLatitudes(11.3426);


//     double_t angle = centreangle(latA, longA, latB, longB);
//     double_t X = bigX(angle, latA, latB);
//     double_t Y = bigY(angle, latA, latB);
//     double_t dist = distance(angle, X, Y);
//     printf("%.6f km\n", dist / 1000);
//     return dist;
// }



#define EARTH_RADIUS 6371.0

double to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}

double vincenty_distance(double lat1, double lon1, double lat2, double lon2) {
    double a = 6378137, b = 6356752.314245, f = 1 / 298.257223563;  // WGS-84 ellipsiod parameters
    double L = to_radians(lon2 - lon1);
    double U1 = atan((1 - f) * tan(to_radians(lat1)));
    double U2 = atan((1 - f) * tan(to_radians(lat2)));
    double sinU1 = sin(U1), cosU1 = cos(U1);
    double sinU2 = sin(U2), cosU2 = cos(U2);
    double lambda = L, lambdaP, iterLimit = 100;
    double cosSqAlpha, sinSigma, cos2SigmaM, cosSigma, sigma, sinLambda, cosLambda;
    do {
        sinLambda = sin(lambda);
        cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2 * sinLambda) * (cosU2 * sinLambda) +
                        (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda) *
                        (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda));
        if (sinSigma == 0) return 0;  // coincident points
        cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1 - sinAlpha * sinAlpha;
        cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;
        double C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1 - C) * f * sinAlpha *
                         (sigma + C * sinSigma *
                         (cos2SigmaM + C * cosSigma *
                         (-1 + 2 * cos2SigmaM * cos2SigmaM)));
    } while (fabs(lambda - lambdaP) > 1e-12 && --iterLimit > 0);

    if (iterLimit == 0) return NAN;  // formula failed to converge

    double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
    double A = 1 + uSq / 16384 * (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
    double B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
    double deltaSigma =
        B * sinSigma *
        (cos2SigmaM + B / 4 *
                        (cosSigma * (-1 + 2 * cos2SigmaM * cos2SigmaM) -
                        B / 6 * cos2SigmaM * (-3 + 4 * sinSigma * sinSigma) *
                        (-3 + 4 * cos2SigmaM * cos2SigmaM)));
    double distance = b * A * (sigma - deltaSigma);

    return distance / 1000.0; // convert to kilometers
}
