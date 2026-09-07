#include <stdio.h>

#include "battlefield.h"
#include "simulation.h"
#include "fileio.h"

// displaying the menu
static void showInstructions(void)
{
    printf("\n++++++++++++ INSTRUCTIONS ++++++++++++\n");
    printf("1. Start Simulation and complete Setup.\n");
    printf("2. Set Battleship properties.\n");
    printf("3. Set Escort ship settings.\n");
    printf("4. Set a seed value.\n");
    printf("5. Generate the battlefield.\n");
    printf("6. Choose Show Simulation and run a part.\n");
    printf("7. Results are saved to simulation_results.txt.\n");
    printf("8. Simulation Statistics displays saved results.\n");
}

// desplaying the setup menu
static void setupMenu(Battlefield *bf)
{
    int choice;

    do
    {
        printf("\n++++++++++++ SETUP ++++++++++++\n");
        printf("1. Battleship Properties\n");
        printf("2. Escort Ships Settings\n");
        printf("3. Seed Value\n");
        printf("4. Generate Battlefield\n");
        printf("5. Return to Main Menu\n");

        printf("Choice: ");
        scanf("%d", &choice);

        // performing an action depending on the selcted optins
        switch (choice)
        {
            case 1:
                setupBattleshipProperties(bf);
                break;

            case 2:
                setupEscortSettings(bf);
                break;

            case 3:
                setupSeed(bf);
                break;

            case 4:
                generateBattlefield(bf);

                if (bf->battlefieldGenerated)
                {
                    displayBattlefield(
                        bf,
                        "INITIAL BATTLEFIELD"
                    );

                    saveBattlefield(
                        bf,
                        "INITIAL BATTLEFIELD"
                    );
                }
                break;

            case 5:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 5);
}

// displaying the different types of simulations available to the user 
static void simulationMenu(Battlefield *bf)
{
    int choice;

    do
    {
        printf("\n++++++++++++ SHOW SIMULATION ++++++++++++\n");
        printf("1. Part 1-A\n");
        printf("2. Part 1-B Simulation 1\n");
        printf("3. Part 1-B Simulation 2\n");
        printf("4. Part 1-C\n");
        printf("5. Part 2-A\n");
        printf("6. Part 2-B\n");
        printf("8. Return\n");

        printf("Choice: ");
        scanf("%d", &choice);

        // performing an action depending on the selected option
        switch (choice)
        {
            case 1:
                runPart1A(bf);
                break;

            case 2:
                runPart1BSimulation1(bf);
                break;

            case 3:
                runPart1BSimulation2(bf);
                break;

            case 4:
                runPart1C(bf);
                break;

            case 5:
                runPart2A(bf);
                break;

            case 6:
                runPart2B(bf);
                break;

            case 7:
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 8);
}

// displaying the setup menu and the simulation menu
static void startSimulationMenu(Battlefield *bf)
{
    int choice;

    do
    {
        printf("\n++++++++++++ START SIMULATION ++++++++++++\n");
        printf("1. Setup\n");
        printf("2. Show Simulation\n");
        printf("3. Return to Main Menu\n");

        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            setupMenu(bf);
        }
        else if (choice == 2)
        {
            simulationMenu(bf);
        }
        else if (choice != 3)
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 3);
}

// creates the battlefield and displays the main menu
int main(void)
{
    Battlefield battlefield;
    int choice;

    initializeBattlefield(
        &battlefield
    );

    // despleying the main menu
    do
    {
        printf("\n");
        printf("ADVANCED NAVAL BATTLE SIMULATOR\n");
        printf("1. Start Simulation\n");
        printf("2. View Instructions\n");
        printf("3. Simulation Statistics\n");
        printf("4. Exit\n");

        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            startSimulationMenu(
                &battlefield
            );
        }
        else if (choice == 2)
        {
            showInstructions();
        }
        else if (choice == 3)
        {
            showStatistics();
        }
        else if (choice == 4)
        {
        }
        else
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 4);

    return 0;
}