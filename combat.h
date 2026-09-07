#ifndef COMBAT_H
#define COMBAT_H

#include "battlefield.h"

double distance2D(double x1, double y1, double x2, double y2);
double projectileRange(double velocity, double angleDeg);

int battleshipCanHit(
    const Battleship *b,
    const EscortShip *e,
    double minimumAngle
);

int escortCanHit(
    const EscortShip *e,
    const Battleship *b
);

double battleshipHitTime(
    const Battleship *b,
    const EscortShip *e,
    double minimumAngle
);

double escortHitTime(
    const EscortShip *e,
    const Battleship *b
);

double degradedImpact(
    double initialImpact,
    double gamma,
    int shotsFired
);

int chooseAttackTarget(
    const Battlefield *bf,
    double minimumAngle
);

void printAttackOrder(
    const Battlefield *bf,
    double minimumAngle
);

#endif