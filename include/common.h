#ifndef COMMON_H
#define COMMON_H

/* ---------- Physical constants ---------- */
#define G 9.8
#define PI 3.14159265358979323846

/* ---------- Simulation limits ---------- */
#define MAX_ESCORTS 100     /* upper bound on N, escort ship count      */
#define MAX_NAME_LEN 50

/* ---------- Escort ship types (Table 1) ---------- */
/* Index into the ESCORT_SPECS lookup table below.   */
typedef enum {
    TYPE_A = 0,   /* 1936A-class Destroyer     */
    TYPE_B = 1,   /* Gabbiano-class Corvette   */
    TYPE_C = 2,   /* Matsu-class Destroyer     */
    TYPE_D = 3,   /* F-class Escort Ships      */
    TYPE_E = 4    /* Japanese Kaibokan         */
} EscortType;

/* Static spec data for each escort type (from Table 1) that does NOT
   change per-simulation: impact power and angle RANGE (theta_H - theta_L).
   Min/max velocity and the actual theta_L are generated per-ship at
   runtime, since the table says these are randomly generated.        */
typedef struct {
    char typeName[MAX_NAME_LEN];
    char gunName[MAX_NAME_LEN];
    double impactPower;   /* e.g. 0.08 for Type A                     */
    double angleRange;    /* theta_H - theta_L, e.g. 20 degrees        */
} EscortSpec;

/* ---------- Battleship ---------- */
typedef struct {
    char notation;         /* 'U', 'M', 'R', or 'S'                    */
    char name[MAX_NAME_LEN];
    double x, y;            /* position on the canvas                   */
    double vMax;            /* max shell speed (user input or random)   */
    double vMin;            /* always 0, per spec                       */
    double gamma;           /* impact-power decay rate (Part 2-C)       */
    double impactPower;     /* current impact power, starts at 1.0      */
    int alive;              /* 1 = still afloat, 0 = sunk                */
} Battleship;

/* ---------- Escort ship ---------- */
typedef struct {
    int id;                 /* unique index number                      */
    EscortType type;        /* which of the 5 types                     */
    double x, y;
    double vMin, vMax;       /* randomly generated, within table rules   */
    double angleMin, angleMax; /* theta_L and theta_H, in degrees        */
    double impactPower;     /* current impact power on B                */
    double gamma;           /* decay rate (Part 2-C)                    */
    int alive;               /* 1 = still afloat, 0 = destroyed          */
    int hasFired;            /* 1 = already used its one shot (Part 1)   */
} EscortShip;

#endif
