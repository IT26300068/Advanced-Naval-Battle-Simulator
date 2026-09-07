#include <stdio.h>

#include "simulation.h"
#include "combat.h"
#include "fileio.h"

// amount of simulated time added during each loop
#define TIME_STEP 0.10

// safety limit that prevents the simulation from running endlessly
#define MAX_TIME 10000.0


// counts how many Escort ships are still alive
static int livingEscortCount(const Battlefield *bf)
{
    int i;
    int count = 0;

    for (i = 0; i < bf->escortCount; i++)
    {
        if (bf->escorts[i].alive)
        {
            count++;
        }
    }

    return count;
}


// makes sure a battlefield has been generated before a simulation 
static void ensureGenerated(Battlefield *bf)
{
    if (bf->battlefieldGenerated == 0)
    {
        generateBattlefield(bf);
    }
}


// reads an integer (0/1)
static int readZeroOrOne(const char *message)
{
    int value;

    do
    {
        printf("%s", message);
        scanf("%d", &value);

        if (value != 0 && value != 1)
        {
            printf("Please enter 0 or 1.\n");
        }

    } while (value != 0 && value != 1);

    return value;
}


// checks whether at least one Escort ship can still fire
static int escortCanStillFire(const Battlefield *bf, int continuousEscortFire)
{
    int i;

    for (i = 0; i < bf->escortCount; i++)
    {
        const EscortShip *e = &bf->escorts[i];

        if (!e->alive)
        {
            continue;
        }

        if (!escortCanHit(e, &bf->b))
        {
            continue;
        }

        if (continuousEscortFire)
        {
            return 1;
        }

        if (e->firedOnce == 0)
        {
            return 1;
        }
    }

    return 0;
}


// runs Part 1-A at the Battleship's current position
static void part1AAtCurrentPosition(Battlefield *bf)
{
    int i;
    int sinkingEscort = -1;
    int hitCount = 0;
    double endTime = 0.0;
    char summary[180];

    for (i = 0; i < bf->escortCount; i++)
    {
        if (bf->escorts[i].alive && escortCanHit(&bf->escorts[i], &bf->b)
        )
        {
            sinkingEscort = i;
            break;
        }
    }

    if (sinkingEscort != -1)
    {
        char text[150];

        bf->b.health = 0.0;
        bf->b.alive = 0;

        printf("Battleship sinks because Escort %d can hit it.\n", bf->escorts[sinkingEscort].id);

        sprintf(text, "Battleship sunk by Escort %d.", bf->escorts[sinkingEscort].id);

        saveText(text);
        return;
    }

    for (i = 0; i < bf->escortCount; i++)
    {
        if (bf->escorts[i].alive && battleshipCanHit(&bf->b, &bf->escorts[i], 0.0))
        {
            double hitTime;
            char text[180];

            hitTime = battleshipHitTime(&bf->b, &bf->escorts[i], 0.0);

            bf->escorts[i].health = 0.0;
            bf->escorts[i].alive = 0;

            hitCount++;

            if (hitTime > endTime)
            {
                endTime = hitTime;
            }

            printf("Escort %d hit. Time: %.2f s\n", bf->escorts[i].id, hitTime);

            sprintf(text, "Escort %d hit. Time: %.2f s", bf->escorts[i].id, hitTime);

            saveText(text);
        }
    }

    printf("Escort ships hit by B: %d\n", hitCount);
    printf("Battle end time: %.2f s\n", endTime);

    sprintf(summary, "Escort ships hit by B: %d", hitCount);
    saveText(summary);

    sprintf(summary, "Battle end time: %.2f s", endTime);
    saveText(summary);
}


// runs Part 1-C at the Battleship's current position
static double part1CAtCurrentPosition(Battlefield *bf, double minimumAngle)
{
    int i;
    double impactThisPosition = 0.0;

    for (i = 0; i < bf->escortCount; i++)
    {
        EscortShip *e = &bf->escorts[i];

        if (e->alive && e->firedOnce == 0 && escortCanHit(e, &bf->b)
        )
        {
            e->firedOnce = 1;
            e->shotsFired++;

            bf->b.health -= e->impactPower;
            impactThisPosition += e->impactPower;

            if (bf->b.health <= 0.0)
            {
                bf->b.health = 0.0;
                bf->b.alive = 0;

                printf("Battleship destroyed by Escort %d.\n", e->id);

                break;
            }
        }
    }

    if (bf->b.alive)
    {
        for (i = 0; i < bf->escortCount; i++)
        {
            if (bf->escorts[i].alive && battleshipCanHit (&bf->b, &bf->escorts[i], minimumAngle))
            {
                bf->escorts[i].health = 0.0;
                bf->escorts[i].alive = 0;
            }
        }
    }

    printf("Impact on B at this position: %.2f%%\n", impactThisPosition * 100.0);

    printf("Battleship health: %.2f%%\n", bf->b.health * 100.0);

    return impactThisPosition;
}


// runs a Part 1 simulation while B moves along
static void runPathPart1(Battlefield *bf, int damageMode, int jammed, int generateNewPath)
{
    int p;
    int jamAfter = -1;
    double jamMinimumAngle = 0.0;
    double totalCumulativeImpact = 0.0;

    if (generateNewPath || bf->pathCount <= 0)
    {
        enterPath(bf);
    }

    if (jammed)
    {
        do
        {
            printf("Enter iteration after which the gun jams " "(0 to %d): ", bf->pathCount - 1);

            scanf("%d", &jamAfter);

        } while (jamAfter < 0 || jamAfter >= bf->pathCount);

        do
        {
            printf("Enter minimum jammed angle " "(0 < angle < 30): ");

            scanf("%lf", &jamMinimumAngle);

        } while (jamMinimumAngle <= 0.0 || jamMinimumAngle >= 30.0);
    }

    for (p = 0; p < bf->pathCount && bf->b.alive; p++)
    {
        double minimumAngle = 0.0;
        char title[100];

        bf->b.x = bf->pathX[p];
        bf->b.y = bf->pathY[p];

        if (jammed && p > jamAfter)
        {
            minimumAngle = jamMinimumAngle;
        }

        printf("\nIteration %d - B at (%.2f, %.2f)\n", p + 1, bf->b.x, bf->b.y);

        if (damageMode)
        {
            totalCumulativeImpact += part1CAtCurrentPosition(bf, minimumAngle);
        }
        else
        {
            int i;
            int sinkingEscort = -1;

            for (i = 0; i < bf->escortCount; i++)
            {
                if (bf->escorts[i].alive && escortCanHit ( &bf->escorts[i], &bf->b))
                {
                    sinkingEscort = i;
                    break;
                }
            }

            if (sinkingEscort != -1)
            {
                bf->b.health = 0.0;
                bf->b.alive = 0;

                printf("Battleship sunk by Escort %d.\n", bf->escorts[sinkingEscort].id);
            }
            else
            {
                for (i = 0; i < bf->escortCount; i++)
                {
                    if (bf->escorts[i].alive && battleshipCanHit (&bf->b, &bf->escorts[i], minimumAngle))
                    {
                        bf->escorts[i].health = 0.0;
                        bf->escorts[i].alive = 0;

                        printf("Escort %d destroyed.\n", bf->escorts[i].id);
                    }
                }
            }
        }

        sprintf(title, "Path Iteration %d", p + 1);
        saveBattlefield(bf, title);
    }

    if (damageMode)
    {
        char text[180];

        printf("Total cumulative impact on B: %.2f%%\n", totalCumulativeImpact * 100.0);

        sprintf(text, "Total cumulative impact on B: %.2f%%", totalCumulativeImpact * 100.0);

        saveText(text);
    }
}

// runs the time stationary battle used by part 2 A and B
static void timedStationaryBattle(Battlefield *bf, int continuousEscortFire, int damageMode)
{
    double time = 0.0;
    double nextBFire = 0.0;
    double nextEFire[MAX_ESCORTS];
    int i;

    for (i = 0; i < bf->escortCount; i++)
    {
        nextEFire[i] = 0.0;
        bf->escorts[i].firedOnce = 0;
    }

    printAttackOrder(bf, 0.0);

    while (bf->b.alive && livingEscortCount(bf) > 0 && time <= MAX_TIME)
    {
        int target;

        target = chooseAttackTarget(bf, 0.0);

        if (target != -1 && time + 0.0001 >= nextBFire)
        {
            double impact = 1.0;

            bf->escorts[target].health -= impact;
            bf->b.shotsFired++;

            if (bf->escorts[target].health <= 0.0)
            {
                bf->escorts[target].health = 0.0;
                bf->escorts[target].alive = 0;
            }

            if (bf->b.reloadTime == 0.0)
            {
                nextBFire = time + TIME_STEP;
            }
            else
            {
                nextBFire = time + bf->b.reloadTime;
            }
        }

        for (i = 0; i < bf->escortCount; i++)
        {
            EscortShip *e = &bf->escorts[i];

            if (e->alive && escortCanHit(e, &bf->b))
            {
                int canFire = 0;

                if (continuousEscortFire)
                {
                    if (time + 0.0001 >= nextEFire[i])
                    {
                        canFire = 1;
                    }
                }
                else if (e->firedOnce == 0)
                {
                    canFire = 1;
                }

                if (canFire)
                {
                    double impact;

                    if (damageMode)
                    {
                        impact = e->impactPower;
                    }
                    else
                    {
                        impact = 1.0;
                    }

                    bf->b.health -= impact;

                    e->shotsFired++;
                    e->firedOnce = 1;

                    if (continuousEscortFire)
                    {
                        if (e->reloadTime == 0.0)
                        {
                            nextEFire[i] = time + TIME_STEP;
                        }
                        else
                        {
                            nextEFire[i] = time + e->reloadTime;
                        }
                    }

                    if (bf->b.health <= 0.0)
                    {
                        bf->b.health = 0.0;
                        bf->b.alive = 0;

                        printf("Battleship destroyed by Escort %d at %.2f s.\n", e->id, time);

                        break;
                    }
                }
            }
        }

        if (!bf->b.alive)
        {
            break;
        }

        if (target == -1 && !escortCanStillFire(bf, continuousEscortFire))
        {
            break;
        }

        time += TIME_STEP;
    }

    printf("Simulation time: %.2f s\n", time);
    printf("Battleship health: %.2f%%\n", bf->b.health * 100.0);

    {
        char text[180];

        sprintf(text, "Simulation time: %.2f s", time);
        saveText(text);

        sprintf(text, "Battleship health: %.2f%%", bf->b.health * 100.0);
        saveText(text);
    }
}


// runs Part 1-A
void runPart1A(Battlefield *bf)
{
    ensureGenerated(bf);

    if (!bf->battlefieldGenerated)
    {
        return;
    }

    resetBattleState(bf);

    saveText("");
    saveText("++++++++++++ PART 1-A ++++++++++++");

    saveBattlefield(
        bf,
        "PART 1-A INITIAL CONDITIONS"
    );

    part1AAtCurrentPosition(bf);

    saveBattlefield(
        bf,
        "PART 1-A FINAL CONDITIONS"
    );
}


// runs Part 1-B simulation 1 using a newly generated path
void runPart1BSimulation1(Battlefield *bf)
{
    double originalX;
    double originalY;

    ensureGenerated(bf);

    if (!bf->battlefieldGenerated)
    {
        return;
    }

    resetBattleState(bf);

    originalX = bf->b.x;
    originalY = bf->b.y;

    saveText("");
    saveText("++++++++++++ PART 1-B SIMULATION 1 ++++++++++++");

    runPathPart1(bf, 0, 0, 1);

    saveBattlefield(bf, "PART 1-B SIMULATION 1 FINAL CONDITIONS");

    bf->b.x = originalX;
    bf->b.y = originalY;
}


// runs Part 1-B simulation 2 using the same path as simulation 1
void runPart1BSimulation2(Battlefield *bf)
{
    double originalX;
    double originalY;

    ensureGenerated(bf);

    if (!bf->battlefieldGenerated)
    {
        return;
    }

    resetBattleState(bf);

    originalX = bf->b.x;
    originalY = bf->b.y;

    saveText("");
    saveText("++++++++++++ PART 1-B SIMULATION 2 ++++++++++++");

    runPathPart1(bf, 0, 1, 0);

    saveBattlefield(bf, "PART 1-B SIMULATION 2 FINAL CONDITIONS");

    bf->b.x = originalX;
    bf->b.y = originalY;
}


// runs part 1-C and allows the user to redo part 1-A or part 1-B
void runPart1C(Battlefield *bf)
{
    int choice;
    double originalX;
    double originalY;

    ensureGenerated(bf);

    if (!bf->battlefieldGenerated)
    {
        return;
    }

    resetBattleState(bf);

    do
    {
        printf("\nPart 1-C\n");
        printf("1. Redo Part 1-A with impact power\n");
        printf("2. Redo Part 1-B Simulation 1 with impact power\n");
        printf("3. Redo Part 1-B Simulation 2 with impact power\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice < 1 || choice > 3)
        {
            printf("Invalid choice. Enter 1, 2, or 3.\n");
        }

    } while (choice < 1 || choice > 3);

    saveText("");
    saveText("++++++++++++ PART 1-C ++++++++++++");

    if (choice == 1)
    {
        double cumulativeImpact;
        char text[180];

        saveBattlefield(
            bf,
            "PART 1-C INITIAL CONDITIONS"
        );

        cumulativeImpact = part1CAtCurrentPosition(bf, 0.0);

        printf("Cumulative impact on B: %.2f%%\n", cumulativeImpact * 100.0);

        sprintf(text, "Cumulative impact on B: %.2f%%", cumulativeImpact * 100.0);

        saveText(text);
    }
    else
    {
        originalX = bf->b.x;
        originalY = bf->b.y;

        if (choice == 2)
        {
            runPathPart1(bf, 1, 0, 0);
        }
        else
        {
            runPathPart1(bf, 1, 1, 0);
        }

        saveBattlefield(bf, "PART 1-C FINAL PATH CONDITIONS");

        bf->b.x = originalX;
        bf->b.y = originalY;
    }

    saveBattlefield(bf, "PART 1-C FINAL CONDITIONS");
}

void runPart2A(Battlefield *bf)
{
    int damageMode;

    ensureGenerated(bf);

    if (!bf->battlefieldGenerated)
    {
        return;
    }

    resetBattleState(bf);

    saveText("");
    saveText("++++++++++++ PART 2-A ++++++++++++");

    damageMode = readZeroOrOne("Use Part 1-C impact power? (1 = Yes, 0 = No): ");

    timedStationaryBattle(bf, 0, damageMode);

    saveBattlefield(bf, "PART 2-A FINAL CONDITIONS");
}

void runPart2B(Battlefield *bf)
{
    int damageMode;

    ensureGenerated(bf);

    if (!bf->battlefieldGenerated)
    {
        return;
    }

    resetBattleState(bf);

    saveText("");
    saveText("++++++++++++ PART 2-B ++++++++++++");

    damageMode = readZeroOrOne("Use Part 1-C impact power? (1 = Yes, 0 = No): ");

    timedStationaryBattle(bf, 1, damageMode);

    saveBattlefield(bf, "PART 2-B FINAL CONDITIONS");
}