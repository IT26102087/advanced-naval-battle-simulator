#include<math.h>
#include<common.h>
#include<physics.h>

double timeOfFlight(double U, double thetaDeg) {
	double thetaRad = thetaDeg*PI/180.0;
	double uy = U*sin(thetaRad);

	return (2.0 * uy)/G;
}

double calRange(double U, double thetaDeg) {
	double thetaRad = thetaDeg * PI / 180.0;

	return (U*U*sin(2.0*thetaRad))/G;
}

double distanceBetween(double x1, double y1, double x2, double y2)  {
	double dx = x2 - x1;
	double dy = y2 - y1;

	return sqrt(dx*dx+dy*dy);
}
