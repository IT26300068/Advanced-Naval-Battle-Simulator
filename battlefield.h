#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#define MAX_ESCORTS 100
#define MAX_PATH_POINTS 50
#define TYPE_LEN 3
#define NAME_LEN 50

typedef struct
{
    char notation[TYPE_LEN];
    char typeName[NAME_LEN];
    char gunName[NAME_LEN];
    double impactPower;
    double angleRange;
    double reloadTime;
    double gamma;
} EscortType;

typedef struct
{
    char notation[2];
    char name[NAME_LEN];
    char gunName[NAME_LEN];

    double x;
    double y;
    double maxVelocity;
    double reloadTime;
    double gamma;

    double health;
    int alive;
    int shotsFired;
} Battleship;

typedef struct
{
    int id;
    char type[TYPE_LEN];

    double x;
    double y;
    double minVelocity;
    double maxVelocity;
    double minAngle;
    double maxAngle;

    double impactPower;
    double reloadTime;
    double gamma;

    double health;
    int alive;
    int shotsFired;
    int firedOnce;
} EscortShip;

typedef struct
{
    double canvasSize;
    int escortCount;
    unsigned long seed;

    Battleship b;
    EscortType escortTypes[5];
    EscortShip escorts[MAX_ESCORTS];

    int pathCount;
    double pathX[MAX_PATH_POINTS];
    double pathY[MAX_PATH_POINTS];

    int battleshipReady;
    int escortSettingsReady;
    int battlefieldGenerated;
} Battlefield;

void initializeBattlefield(Battlefield *bf);
void setupBattleshipProperties(Battlefield *bf);
void setupEscortSettings(Battlefield *bf);
void setupSeed(Battlefield *bf);
void generateBattlefield(Battlefield *bf);
void enterPath(Battlefield *bf);
void resetBattleState(Battlefield *bf);
void displayBattlefield(const Battlefield *bf, const char title[]);

#endif