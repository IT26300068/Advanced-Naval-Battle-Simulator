#include <stdio.h>
#include <string.h>

#include "battlefield.h"

// generates the next number using the current seed value
static unsigned long nextRandom(Battlefield *bf)
{
    bf->seed = bf->seed * 1103515245UL + 12345UL;
    return bf->seed;
}

// generates a random integer
static int randomInt(Battlefield *bf, int minimum, int maximum)
{
    unsigned long value;

    value = nextRandom(bf);

    return minimum + (int)(value % (unsigned long)(maximum - minimum + 1));
}

// generates a random decimal number
static double randomDouble(
    Battlefield *bf,
    double minimum,
    double maximum)
{
    unsigned long value;
    double fraction;

    value = nextRandom(bf);

    fraction = (double)(value % 10001UL) / 10000.0;

    return minimum + fraction * (maximum - minimum);
}

static void setBattleshipName(Battleship *b)
{
    if (strcmp(b->notation, "U") == 0)
    {
        strcpy(b->name, "USS Iowa (BB-61)");
        strcpy(b->gunName, "50-caliber Mark 7 gun");
    }
    else if (strcmp(b->notation, "M") == 0)
    {
        strcpy(b->name, "MS King George V");
        strcpy(b->gunName, "(356 mm) Mark VII gun");
    }
    else if (strcmp(b->notation, "R") == 0)
    {
        strcpy(b->name, "Richelieu");
        strcpy(b->gunName, "(15 inch) Mle 1935 gun");
    }
    else
    {
        strcpy(b->notation, "S");
        strcpy(b->name, "Sovetsky Soyuz-class");
        strcpy(b->gunName, "(16 inch) B-37 gun");
    }
}

// sets the starting values of the attlefield that user gave and stores it in to th five escort ships
void initializeBattlefield(Battlefield *bf)
{
    int i;

    bf->canvasSize = 0;
    bf->escortCount = 0;
    bf->seed = 1;
    bf->pathCount = 0;

    bf->battleshipReady = 0;
    bf->escortSettingsReady = 0;
    bf->battlefieldGenerated = 0;

    strcpy(bf->escortTypes[0].notation, "EA");
    strcpy(bf->escortTypes[0].typeName, "1936A-class Destroyer");
    strcpy(bf->escortTypes[0].gunName, "SK C/34 naval gun");
    bf->escortTypes[0].impactPower = 0.08;
    bf->escortTypes[0].angleRange = 20;

    strcpy(bf->escortTypes[1].notation, "EB");
    strcpy(bf->escortTypes[1].typeName, "Gabbiano-class Corvette");
    strcpy(bf->escortTypes[1].gunName, "L/47 dual-purpose gun");
    bf->escortTypes[1].impactPower = 0.06;
    bf->escortTypes[1].angleRange = 30;

    strcpy(bf->escortTypes[2].notation, "EC");
    strcpy(bf->escortTypes[2].typeName, "Matsu-class Destroyer");
    strcpy(bf->escortTypes[2].gunName, "Type 89 dual-purpose gun");
    bf->escortTypes[2].impactPower = 0.07;
    bf->escortTypes[2].angleRange = 25;

    strcpy(bf->escortTypes[3].notation, "ED");
    strcpy(bf->escortTypes[3].typeName, "F-class Escort Ship");
    strcpy(bf->escortTypes[3].gunName, "SK C/32 naval gun");
    bf->escortTypes[3].impactPower = 0.05;
    bf->escortTypes[3].angleRange = 50;

    strcpy(bf->escortTypes[4].notation, "EE");
    strcpy(bf->escortTypes[4].typeName, "Japanese Kaibokan");
    strcpy(bf->escortTypes[4].gunName, "4.7 inch naval gun");
    bf->escortTypes[4].impactPower = 0.04;
    bf->escortTypes[4].angleRange = 70;

    for (i = 0; i < 5; i++)
    {
        bf->escortTypes[i].reloadTime = 1.0;
        bf->escortTypes[i].gamma = 0.05;
    }
}

// gets the battleship type, battlefield size, velocity, reload time and gamma form the user
void setupBattleshipProperties(Battlefield *bf)
{
    printf("\n+++++ Battleship Properties +++++\n");
    printf("U - USS Iowa (BB-61)\n");
    printf("M - MS King George V\n");
    printf("R - Richelieu\n");
    printf("S - Sovetsky Soyuz-class\n");

    do
    {
        printf("Enter Battleship type: ");
        scanf("%1s", bf->b.notation);

    } while (
        strcmp(bf->b.notation, "U") != 0 &&
        strcmp(bf->b.notation, "M") != 0 &&
        strcmp(bf->b.notation, "R") != 0 &&
        strcmp(bf->b.notation, "S") != 0
    );

    setBattleshipName(&bf->b);

    do
    {
        printf("Enter square battlefield size D (> 0): ");
        scanf("%lf", &bf->canvasSize);

    } while (bf->canvasSize <= 0);

    do
    {
        printf("Enter Battleship maximum shell velocity (> 0): ");
        scanf("%lf", &bf->b.maxVelocity);

    } while (bf->b.maxVelocity <= 0);

    do
    {
        printf("Enter Battleship reload time (>= 0): ");
        scanf("%lf", &bf->b.reloadTime);

    } while (bf->b.reloadTime < 0);

    do
    {
        printf("Enter Battleship gamma (>= 0): ");
        scanf("%lf", &bf->b.gamma);

    } while (bf->b.gamma < 0);

    bf->battleshipReady = 1;
    bf->battlefieldGenerated = 0;

    printf("Battleship properties saved.\n");
}

// getting the number of the escort ship and personalized setting from the user
void setupEscortSettings(Battlefield *bf)
{
    int i;

    printf("\n+++++ Escort Ship Settings +++++\n");

    do
    {
        printf("Enter number of Escort ships (1-%d): ", MAX_ESCORTS);
        scanf("%d", &bf->escortCount);

    } while (
        bf->escortCount < 1 ||
        bf->escortCount > MAX_ESCORTS
    );

    for (i = 0; i < 5; i++)
    {
        printf(
            "\n%s - %s\n",
            bf->escortTypes[i].notation,
            bf->escortTypes[i].typeName
        );

        printf("Impact Power: %.2f\n",
               bf->escortTypes[i].impactPower);

        printf("Angle Range: %.2f degrees\n",
               bf->escortTypes[i].angleRange);

        do
        {
            printf("Enter reload time for this type (>= 0): ");
            scanf("%lf", &bf->escortTypes[i].reloadTime);

        } while (bf->escortTypes[i].reloadTime < 0);

        do
        {
            printf("Enter gamma for this type (>= 0): ");
            scanf("%lf", &bf->escortTypes[i].gamma);

        } while (bf->escortTypes[i].gamma < 0);
    }

    bf->escortSettingsReady = 1;
    bf->battlefieldGenerated = 0;

    printf("Escort settings saved.\n");
}

// stores the seed value from the random number generator
void setupSeed(Battlefield *bf)
{
    printf("\n+++++ Seed Value +++++\n");
    printf("Enter a positive seed value: ");
    scanf("%lu", &bf->seed);

    if (bf->seed == 0)
    {
        bf->seed = 1;
    }

    bf->battlefieldGenerated = 0;

    printf("Seed saved.\n");
}

// random generatoe for ship type, position, angle and the shell velocity
void generateBattlefield(Battlefield *bf)
{
    int i;

    if (
        bf->battleshipReady == 0 ||
        bf->escortSettingsReady == 0
    )
    {
        printf(
            "\nComplete Battleship Properties and "
            "Escort Ship Settings first.\n"
        );

        return;
    }

    bf->b.x = randomDouble(bf, 0, bf->canvasSize);
    bf->b.y = randomDouble(bf, 0, bf->canvasSize);

    bf->b.health = 1.0;
    bf->b.alive = 1;
    bf->b.shotsFired = 0;

    for (i = 0; i < bf->escortCount; i++)
    {
        int typeIndex;
        double highestMinimumAngle;
        double maximumVelocityLimit;

        typeIndex = randomInt(bf, 0, 4);

        bf->escorts[i].id = i + 1;

        strcpy(
            bf->escorts[i].type,
            bf->escortTypes[typeIndex].notation
        );

        bf->escorts[i].x =
            randomDouble(bf, 0, bf->canvasSize);

        bf->escorts[i].y =
            randomDouble(bf, 0, bf->canvasSize);

        highestMinimumAngle =
            90.0 - bf->escortTypes[typeIndex].angleRange;

        bf->escorts[i].minAngle =
            randomDouble(bf, 0, highestMinimumAngle);

        bf->escorts[i].maxAngle =
            bf->escorts[i].minAngle +
            bf->escortTypes[typeIndex].angleRange;

        if (typeIndex == 0)
        {
            maximumVelocityLimit =
                1.2 * bf->b.maxVelocity;
        }
        else
        {
            maximumVelocityLimit =
                0.95 * bf->b.maxVelocity;
        }

        bf->escorts[i].maxVelocity =
            randomDouble(
                bf,
                0.40 * maximumVelocityLimit,
                maximumVelocityLimit
            );

        bf->escorts[i].minVelocity =
            randomDouble(
                bf,
                0,
                bf->escorts[i].maxVelocity
            );

        bf->escorts[i].impactPower =
            bf->escortTypes[typeIndex].impactPower;

        bf->escorts[i].reloadTime =
            bf->escortTypes[typeIndex].reloadTime;

        bf->escorts[i].gamma =
            bf->escortTypes[typeIndex].gamma;

        bf->escorts[i].health = 1.0;
        bf->escorts[i].alive = 1;
        bf->escorts[i].shotsFired = 0;
        bf->escorts[i].firedOnce = 0;
    }

    bf->battlefieldGenerated = 1;

    printf("\nBattlefield generated from the seed.\n");
}

// generates path points for the battleship inside the battlefield
void enterPath(Battlefield *bf)
{
    int i;

    do
    {
        printf("Enter number of path points k (1-%d): ", MAX_PATH_POINTS);

        scanf("%d", &bf->pathCount);

    } while (bf->pathCount < 1 || bf->pathCount > MAX_PATH_POINTS);

    for (i = 0; i < bf->pathCount; i++)
    {
        bf->pathX[i] =
            randomDouble(bf, 0, bf->canvasSize);

        bf->pathY[i] =
            randomDouble(bf, 0, bf->canvasSize);
    }

    printf("Path points generated.\n");
}

// resets the health, alive status shots before anew simulation
void resetBattleState(Battlefield *bf)
{
    int i;

    bf->b.health = 1.0;
    bf->b.alive = 1;
    bf->b.shotsFired = 0;

    for (i = 0; i < bf->escortCount; i++)
    {
        bf->escorts[i].health = 1.0;
        bf->escorts[i].alive = 1;
        bf->escorts[i].shotsFired = 0;
        bf->escorts[i].firedOnce = 0;
    }
}

// displays the battleship and escort ship information
void displayBattlefield(
    const Battlefield *bf,
    const char title[])
{
    int i;

    printf("\n++++++++++++++++++++++++++++++\n");
    printf("%s\n", title);
    printf("++++++++++++++++++++++++++++++\n");

    printf(
        "Canvas: (0,0) to (%.2f, %.2f)\n",
        bf->canvasSize,
        bf->canvasSize
    );

    printf("\nBattleship\n");
    printf("Type: %s\n", bf->b.notation);
    printf("Name: %s\n", bf->b.name);
    printf("Gun: %s\n", bf->b.gunName);
    printf("Position: (%.2f, %.2f)\n", bf->b.x, bf->b.y);
    printf("Vmax: %.2f\n", bf->b.maxVelocity);
    printf("Reload Time: %.2f\n", bf->b.reloadTime);
    printf("Gamma: %.4f\n", bf->b.gamma);
    printf("Health: %.2f%%\n", bf->b.health * 100.0);
    printf("Shots Fired: %d\n", bf->b.shotsFired);

    for (i = 0; i < bf->escortCount; i++)
    {
        const EscortShip *e;

        e = &bf->escorts[i];

        printf("\nEscort %d\n", e->id);
        printf("Type: %s\n", e->type);
        printf("Position: (%.2f, %.2f)\n", e->x, e->y);
        printf("Velocity: %.2f - %.2f\n", e->minVelocity, e->maxVelocity);
        printf("Angle: %.2f - %.2f\n", e->minAngle, e->maxAngle);
        printf("Impact Power: %.2f\n", e->impactPower);
        printf("Reload Time: %.2f\n", e->reloadTime);
        printf("Gamma: %.4f\n", e->gamma);
        printf("Health: %.2f%%\n", e->health * 100.0);
        printf("Status: %s\n", e->alive ? "Alive" : "Destroyed");
    }
}