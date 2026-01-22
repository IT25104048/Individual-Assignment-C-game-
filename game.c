#include <stdio.h>
#include <stdlib.h>

/* Function prototypes */
void singlePlayerGame();
void twoPlayerGame();
void threePlayerGame();

int main()
{
    int choice;

    printf("=====================================\n");
    printf("   SpyNet – The Codebreaker Protocol  \n");
    printf("=====================================\n");
    printf("Select Game Mode:\n");
    printf("1. Single Player\n");
    printf("2. Two Players\n");
    printf("3. Three Players\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            printf("\nStarting Single Player Game...\n");
            singlePlayerGame();
            break;

        case 2:
            printf("\nStarting Two Player Game...\n");
            twoPlayerGame();
            break;

        case 3:
            printf("\nStarting Three Player Game...\n");
            threePlayerGame();
            break;

        default:

	    printf("\nInvalid choice! Please run the program again.\n");
            break;
    }

    return 0;
}
// select the game

void singlePlayerGame()
{
    printf("Single Player logic goes here.\n");
}

void twoPlayerGame()
{
    printf("Two Player logic goes here.\n");
}

void threePlayerGame()
{
    printf("Three Player logic goes here.\n");
}

