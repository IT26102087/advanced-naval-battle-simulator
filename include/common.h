#ifndef common_h
#define common_h

#define G 9.8
#define PI 3.1415

#define MAX_NAME_LEN 50
#define MAX_ESCORTS 100

typedef enum {
	TYPE_A = 0,
	TYPE_B = 1,
	TYPE_C = 2,
	TYPE_D = 3,
	TYPE_E = 4,
}EscortType;

typedef struct{
	char notation;
	char name[MAX_NAME_LEN];
	double x, y;
	double vMax;
	double vMin;
	double gamma;
	double impactPower;
	int alive;
} Battleship;

typedef struct {
	int id,type;
	double x, y;
	double vMin;
	double vMax;
	double angleMin, angleMax;
	double impactPower;
	double gamma;
	int alive;
	int hasFired;
} Escortship;



#endif
