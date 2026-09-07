#include <stdio.h>
#include <math.h>

#include "combat.h"
#include "fileio.h"

// value of PI to convert degrees into radians
#define PI 3.14
// gravitational accelaration
#define G 9.81

// calculate the straight line distance 
double distance2D(
    double x1,
    double y1,
    double x2,
    double y2)
{
    double dx;
    double dy;

    // find the difference between the x cordinates
    dx = x2 - x1;
    // find the difference between the y codinates
    dy = y2 - y1;

    // distance
    return sqrt(dx * dx + dy * dy);
}

// calculates how far a projectile can travel
double projectileRange(
    double velocity,
    double angleDeg)
{
    double radians;

    // convert angles from degree to radians
    radians = angleDeg * PI / 180.0;

    // projectile range formula
    return
        velocity * velocity * sin(2.0 * radians) / G;
}

// checks weather the battleship can hit the escort ship or not 
int battleshipCanHit(
    const Battleship *b,
    const EscortShip *e,
    double minimumAngle)
{
    double distance;
    double angle;

    if (!b->alive || !e->alive)
    {
        return 0;
    }

    // distacne between the battleship and escortship
    distance = distance2D(b->x, b->y, e->x, e->y);

    // strating from minimum angle up to 90
    for (angle = minimumAngle; angle <= 90.0; angle += 0.5)
    {
        // checks the target distance
        if (distance <= projectileRange (b->maxVelocity, angle)
        )
        {
            return 1;
        }
    }

    return 0;
}

// checks weather an escort ship can hit the battleship
int escortCanHit(const EscortShip *e, const Battleship *b)
{
    double distance;
    double angle;

    if (!e->alive || !b->alive)
    {
        return 0;
    }

    distance = distance2D(e->x, e->y, b->x, b->y);

    for (angle = e->minAngle; angle <= e->maxAngle; angle += 0.5)
    {
        double lowRange;
        double highRange;

        lowRange =
            projectileRange(
                e->minVelocity,
                angle
            );

        highRange =
            projectileRange(
                e->maxVelocity,
                angle
            );

        if (
            distance >= lowRange &&
            distance <= highRange
        )
        {
            return 1;
        }
    }

    return 0;
}

// finds the shortest time to hit the escort ship from battleship
double battleshipHitTime(
    const Battleship *b,
    const EscortShip *e,
    double minimumAngle)
{
    double distance;
    double angle;
    double bestTime;

    distance =
        distance2D(
            b->x,
            b->y,
            e->x,
            e->y
        );

    bestTime = -1;

    for (angle = minimumAngle;
         angle <= 90.0;
         angle += 0.5)
    {
        double radians;
        double sineValue;
        double requiredVelocity;
        double time;

        radians =
            angle *
            PI /
            180.0;

        sineValue =
            sin(2.0 * radians);

        if (sineValue > 0)
        {
            requiredVelocity =
                sqrt(
                    distance *
                    G /
                    sineValue
                );

            if (
                requiredVelocity <=
                b->maxVelocity
            )
            {
                time =
                    2.0 *
                    requiredVelocity *
                    sin(radians) /
                    G;

                if (
                    bestTime < 0 ||
                    time < bestTime
                )
                {
                    bestTime = time;
                }
            }
        }
    }

    return bestTime;
}

// finds the shortest time to hit the battleship from an escort ship
double escortHitTime(
    const EscortShip *e,
    const Battleship *b)
{
    double distance;
    double angle;
    double bestTime;

    distance =
        distance2D(
            e->x,
            e->y,
            b->x,
            b->y
        );

    bestTime = -1;

    for (angle = e->minAngle; angle <= e->maxAngle; angle += 0.5)
    {
        double radians;
        double sineValue;
        double requiredVelocity;
        double time;

        radians = angle * PI / 180.0;

        sineValue = sin(2.0 * radians);

        if (sineValue > 0)
        {
            requiredVelocity = sqrt( distance * G / sineValue);

            if (requiredVelocity >= e->minVelocity &&
                requiredVelocity <= e->maxVelocity)
            {
                time = 2.0 * requiredVelocity * sin(radians) / G;

                if ( bestTime < 0 || time < bestTime)
                {
                    bestTime = time;
                }
            }
        }
    }

    return bestTime;
}

// reduce the wepons imapct power 
double degradedImpact(
    double initialImpact,
    double gamma,
    int shotsFired)
{
    return
        initialImpact *
        exp(-gamma * shotsFired);
}

int chooseAttackTarget(
    const Battlefield *bf,
    double minimumAngle)
{
    int i;
    int bestIndex;
    int bestThreat;
    double bestImpact;
    double bestDistance;

    bestIndex = -1;
    bestThreat = -1;
    bestImpact = -1;
    bestDistance = 0;

    for (i = 0; i < bf->escortCount; i++)
    {
        const EscortShip *e;
        int threat;
        double distance;

        e = &bf->escorts[i];

        if (e->alive && battleshipCanHit( &bf->b, e,minimumAngle)
        )
        {
            threat = escortCanHit( e, &bf->b);

            distance = distance2D(bf->b.x, bf->b.y, e->x, e->y);

            if (bestIndex == -1 || threat > bestThreat || (threat == bestThreat && e->impactPower > bestImpact) ||
                (
                    threat == bestThreat &&
                    e->impactPower == bestImpact &&
                    distance < bestDistance
                )
            )
            {
                bestIndex = i;
                bestThreat = threat;
                bestImpact = e->impactPower;
                bestDistance = distance;
            }
        }
    }

    return bestIndex;
}

// prints the order which the battleship should attack an escort ship
void printAttackOrder(const Battlefield *bf, double minimumAngle)
{
    Battlefield copy;
    int order;

    copy = *bf;
    order = 1;

    printf("\nAttack Order\n");
    saveText("Attack Order");

    while (1)
    {
        int target;
        char text[150];

        target = chooseAttackTarget (&copy, minimumAngle);

        if (target == -1)
        {
            break;
        }

        printf("%d. Escort %d (%s)\n", order, copy.escorts[target].id, copy.escorts[target].type);

        sprintf(text, "%d. Escort %d (%s)", order, copy.escorts[target].id, copy.escorts[target].type);

        saveText(text);

        copy.escorts[target].alive = 0;
        order++;
    }
}