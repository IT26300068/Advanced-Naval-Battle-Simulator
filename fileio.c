#include <stdio.h>

#include "fileio.h"

// name of the file were simulation data stored
#define RESULT_FILE "simulation_results.txt"

// saves one line of text into the results file
void saveText(const char text[])
{
    FILE *file;

    file =
        fopen(RESULT_FILE, "a");

    if (file != NULL)
    {
        fprintf(file, "%s\n", text);

        fclose(file);
    }
}
// saves all battlefield information data into the simulation results file
void saveBattlefield(const Battlefield *bf, const char title[])
{
    FILE *file;
    int i;

    file =
        fopen(RESULT_FILE, "a");

    if (file == NULL)
    {
        return;
    }

    fprintf(file, "\n++++++++++++++++++++++++++++++\n");
    fprintf(file, "%s\n", title);
    fprintf(file, "++++++++++++++++++++++++++++++\n");

    fprintf(file, "Canvas: (0,0) to (%.2f, %.2f)\n", bf->canvasSize, bf->canvasSize);

    // saves the battleship type
    fprintf(file, "Battleship Type: %s\n", bf->b.notation);

    // saves the battleship position (x, y)
    fprintf(file, "Battleship Position: (%.2f, %.2f)\n", bf->b.x, bf->b.y);

    // saves the battleships maximum velocity
    fprintf(file, "Battleship Vmax: %.2f\n", bf->b.maxVelocity);

    // saves the battleship health as a percentage
    fprintf(file, "Battleship Health: %.2f%%\n", bf->b.health * 100.0);

    // saves the number of shots fired by the battleship
    fprintf(file, "Battleship Shots: %d\n", bf->b.shotsFired);

    for (i = 0; i < bf->escortCount; i++)
    {
        const EscortShip *e;

        e = &bf->escorts[i];

        // saves the escort ship type
        fprintf(file, "\nEscort %d\n", e->id);
        fprintf(file, "Type: %s\n", e->type);

        // save thw escort ships position
        fprintf(file, "Position: (%.2f, %.2f)\n", e->x, e->y);

        // save the escort ships mminimum and maximum velocity
        fprintf(file, "Velocity: %.2f - %.2f\n", e->minVelocity, e->maxVelocity);

        // saves the escort ships minimum and maximum firing angles
        fprintf(file, "Angle: %.2f - %.2f\n", e->minAngle, e->maxAngle);

        // saves the escort ships impact power 
        fprintf(file, "Impact Power: %.4f\n", e->impactPower);

        // saves the health of the escor ship as a percentage
        fprintf(file, "Health: %.2f%%\n", e->health * 100.0);

        // saves how many shots has fired by the escort ship
        fprintf(file, "Shots: %d\n", e->shotsFired);

        // saves the alive or destroyed status
        fprintf(file, "Status: %s\n", e->alive ? "Alive" : "Destroyed");
    }

    fclose(file);
}

// reads the simulation results and displays it everything on the screen
void showStatistics(void)
{
    FILE *file;
    char line[300];

    file = fopen(RESULT_FILE, "r");

    if (file == NULL)
    {
        printf("\nNo previous simulation results found.\n");
        return;
    }

    printf("\n++++++++++ Simulation Statistics ++++++++++\n");

    while (
        fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }

    fclose(file);
}