#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MIN 5
#define MAX 15

// Keeping all player-related info together makes life easier later
typedef struct {
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

int main (){
	int n,choice, numPlayers;
	srand(time(null));/*Ensures different layouts every run
                          Without this, rand() repeats patterns*/
      
	printf("=====================================\n");
        printf("   SpyNet – The Codebreaker Protocol  \n");
        printf("=====================================\n");
	
   // Grid Size Selecting 
    do {
        printf("Enter grid size (5-15): ");
        if (scanf("%d", &n) != 1) {
            while(getchar() != '\n');
            n = 0;
            continue;
	    }
   }while (n < MIN || n > MAX);	
		
   //foe player node selecting
    printf("\nSelect Game Mode:\n");
    printf("1. Single Player (@)\n");
    printf("2. Two Players (@ & &)\n");
    printf("3. Three Players (@, &, $)\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &choice);

    numPlayers = (choice >= 1 && choice <= 3) ? choice : 1;
  
     // Initialize Players with unique symbols
    Player players[3] = {
        {0, 0, 3, 0, 1, '@'},  // Player 1
        {0, n-1, 3, 0, 1, '&'},  // Player 2
        {n-1, 0, 3, 0, 1, '$'}     // Player 3
    };

    //Setup Grid
    char **grid = createGrid(n);
    initializeGrid(grid, n);
    placeRandom(grid, n, '#', n); //Random Walls
    placeRandom(grid, n, 'I', 3); //3 Intel
    placeRandom(grid, n, 'L', 2); //2 Lives
    placeRandom(grid, n, 'X', 1); //1 Extration

    for(int i = 0; i < numPlayers; i++) grid[players[i].px][players[i].py] = players[i].symbol;//Detect automatic wins & full elimination

//Game Loop 
int currentTurn = 0;
int gameRunning = 1;//1 means the game is active,0 means it should stop

while (gameRunning) {
    int activeCount =0;
    for (int i = 0; i < numPlayers; i++) {
        if (players[i].active) activeCount++;
    }

    // --- Win/Loss Condition Checks ---
    if (numPlayers > 1 && activeCount ==1) {
        for (int i = 0; i < numPlayers; i++) {
            if (players[i].active) {
                printf("\nPlayer %c is the last one standing! Victory by default.\n", players[i].symbol);
            } }
        gameRunning = 0;
    } 
    else if (activeCount == 0) {
        printf("\nAll agents eliminated. Mission Failure.\n");
        gameRunning = 0;
    }

    //Turn Execution
    if (gameRunning) {
        //Use a loop to skip inactive players (Replaces 'continue')
        while (!players[currentTurn].active) {
            currentTurn = (currentTurn + 1) % numPlayers;
        }

        displayGrid(grid, n, players, numPlayers);
        printf("\nPlayer %c's Turn | Health: %d | Intel: %d/3\n",
                players[currentTurn].symbol, players[currentTurn].lives, players[currentTurn].intel);
        printf("Move (W/A/S/D) or Q to quit: ");
        
        char move;
        if (scanf(" %c",&move) == 1) {//Basic safety check for input
            if (move == 'q' || move == 'Q') {
                players[currentTurn].active = 0; 
                grid[players[currentTurn].px][players[currentTurn].py] = '.';
            } else {
                int result = movePlayer(grid, n, &players[currentTurn], move);
                if (result == 2) {
                    gameRunning = 0; //Replaces 'break' (Standard Win)
                }}}

        //Only log and update turn if the move didn't end the game
        if (gameRunning) {
            logState(grid, n, players, currentTurn);
            currentTurn = (currentTurn + 1) % numPlayers;
            }}}//check again

freeGrid(grid, n);
return 0;
}
  //FUNCTIONS
int movePlayer(char **grid, int n, Player *p, char move)
{
    int nextX = p->px;
    int nextY = p->py;

    /* decide movement direction */
    if (move == 'w' || move == 'W')
        nextX--;
    else if (move == 's' || move == 'S')
        nextX++;
    else if (move == 'a' || move == 'A')
        nextY--;
    else if (move == 'd' || move == 'D')
        nextY++;
    else {
        /* invalid key pressed */
        p->lives--;
        return 0;
    }

    /* check boundaries and walls */
    if (nextX < 0 || nextY < 0 || nextX >= n || nextY >= n || grid[nextX][nextY] == '#') {
        printf("Invalid move! Penalty: -1 Life.\n");
        p->lives--;

        if (p->lives <= 0) {
            p->active = 0;
            grid[p->px][p->py] = '.';
        }
        return 0;
    }

    /* collect items if any */
    if (grid[nextX][nextY] == 'I')
        p->intel++;

    if (grid[nextX][nextY] == 'L')
        p->lives++;

    /* extraction point logic */
    if (grid[nextX][nextY] == 'X') {
        if (p->intel >= 3) {
            printf("\nPlayer %c wins! Extraction Successful.\n", p->symbol);
            return 2;
        } else {
            printf("\nPlayer %c failed extraction (Need 3 Intel) and is eliminated!\n", p->symbol);
            p->active = 0;
            grid[p->px][p->py] = '.';
            return 0;
        }
    }

    /* update player position on the grid */
    grid[p->px][p->py] = '.';
    p->px = nextX;
    p->py = nextY;
    grid[p->px][p->py] = p->symbol;

    return 1;
}

