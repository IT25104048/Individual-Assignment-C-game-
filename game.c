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
        int isAI;//0 =Human,1 =Computer

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
        int n,choice, numPlayers,subChoice;
        srand(time(NULL));/*Ensures different layouts every run
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

// Sub-mode selection using isAI flag
    if (choice == 2){      //for 2 player
     printf("\nSelect two player mode:\n");
        printf("1. Human vs Human\n");
        printf("2. Human vs Computer\n");
     printf("Enter your choice: ");
     scanf("%d", &subChoice);
        if (subChoice == 2) players[1].isAI = 1;
    } 
    else if (choice == 3){  //for 3 player
     printf("\nSelect three player mode:\n");
        printf("1. Human vs Human vs Human\n");
        printf("2. Human vs Human vs Computer\n");
        printf("3. Human vs Computer vs Computer\n");
     printf("Enter choice: ");
     scanf("%d", &subChoice);
        if (subChoice == 2) {
            players[2].isAI = 1;
        }
      else if (subChoice == 3) {
          players[1].isAI = 1;
           players[2].isAI = 1;
        }}

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
               printf("\nPlayer %c (%s) is the last one standing! Victory.\n",players[i].symbol, players[i].isAI ? "Computer" : "Human");
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
        printf("\nPlayer %c's Turn(%s) | Health: %d | Intel: %d/3\n",
                players[currentTurn].symbol,players[currentTurn].isAI ? "Computer" : "Human", players[currentTurn].lives, players[currentTurn].intel);

        char move;
	if (players[currentTurn].isAI) {
                // Computer logic: Simple random choice
                char aiMoves[] = {'W', 'A', 'S', 'D'};
                move = aiMoves[rand() % 4];
                printf("Computer chooses move: %c\n", move);
            } else {
                printf("Move (W/A/S/D) or Q to quit: ");
                scanf(" %c", &move);
            }

            if (move == 'q' || move == 'Q') {
                players[currentTurn].active = 0;
                grid[players[currentTurn].px][players[currentTurn].py] = '.';
            } else {
                int result = movePlayer(grid, n, &players[currentTurn], move);
                if (result == 2) {
                    gameRunning = 0; //Replaces 'break' (Standard Win)
                }}

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
            printf("\nPlayer %c (%s) wins! Extraction Successful.\n", 
                   p->symbol, p->isAI ? "Computer" : "Human");

            return 2;
        } else {
            printf("\nPlayer %c failed extraction (Need 3 Intel) and is eliminated!\n", p->symbol);
            p->active = 0;
            grid[p->px][p->py] = '.';
            return 0;
        }}

    /* update player position on the grid */
    grid[p->px][p->py] = '.';
    p->px = nextX;
    p->py = nextY;
    grid[p->px][p->py] = p->symbol;

    return 1;
}
char **createGrid(int n)
{
    char **grid = malloc(n * sizeof(char *));   // allocate rows
    for (int i = 0; i < n; i++) {
        grid[i] = malloc(n * sizeof(char));     // allocate columns
    }
    return grid;
}

void initializeGrid(char **grid, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            grid[i][j] = '.';   // empty cell
        }}}

void displayGrid(char **grid, int n, Player players[], int numPlayers)
{
    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }}

void placeRandom(char **grid, int n, char symbol, int count)
{
    while (count > 0) {
        int x = rand() % n;
        int y = rand() % n;

        if (grid[x][y] == '.') {
            grid[x][y] = symbol;
            count--;
        }}}

void logState(char **grid, int n, Player players[], int turn)
{
    FILE *fp = fopen("game_log.txt", "a");
    if (fp == NULL)
        return;

    fprintf(fp,"Player %c (%s) moved | Health: %d | Intel: %d\n",
            players[turn].symbol,
	    players[turn].isAI ? "AI" : "Human",
            players[turn].lives,
            players[turn].intel);

    fclose(fp);
}

void freeGrid(char **grid, int n)
{
    for (int i = 0; i < n; i++) {
        free(grid[i]);
    }
    free(grid);
}


