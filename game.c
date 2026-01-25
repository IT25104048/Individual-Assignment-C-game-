#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MIN 5
#define MAX 15

// Keeping all player-related info together makes life easier later
typedef struct {
	i
        int px, py; // current position on grid
        int lives; // health points
        int intel; // intel collected so far
        int active; // 1 = still playing, 0 = eliminated
        char symbol; // how this player shows up on the grid
  } Player;
/*Function Prototypes*/
char **createGrid(int n);
void initializeGrid(char **grid, int n);
void displayGrid(char **grid, int n, Player players[], int numPlayers);
void placeRandom(char **grid, int n, char symbol, int count);
int movePlayer(char **grid, int n, Player *p, char move);
void logState(char **grid, int n, Player players[], int turn);
void freeGrid(char **grid, int n);


