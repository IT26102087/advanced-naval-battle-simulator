#include<stdio.h>
#include<physics.h>

int main()
{
	double r = calRange(100,45);
	double t = timeOfFlight(100,45);
	double d = distanceBetween(0, 0, 3, 4);

	printf("Range at 100 m/s, 45 deg: %.2f\n", r);
	printf("Time of flight: %.2f\n", t);
	printf("Distance (0,0)-(3,4): %.2f (shuld be 5,00)\n", d);

	return 0;
}
