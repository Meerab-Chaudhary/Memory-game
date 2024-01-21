#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Macros
#define GRID_SIZE 4
#define BOX_SIZE 100

// Function declarations
Color getcolor(int);
void initializegrid();
int initialwindow(int x,int y);
// Structures
typedef struct {
    Rectangle rec;
    Color color;
    bool picked;
    bool matched;
    bool selected;
} Box;

// Arrays for game elements
Box grid[GRID_SIZE][GRID_SIZE];
Box uniquecolors[GRID_SIZE * GRID_SIZE / 2];
Box originalcolors[GRID_SIZE][GRID_SIZE];

int main(void) 
{
    // Initialization
    const int screenWidth = GRID_SIZE * BOX_SIZE + 250;
    const int screenHeight = GRID_SIZE * BOX_SIZE;

    InitWindow(screenWidth, screenHeight, "Memory Matching Game");
    
    InitAudioDevice();

    SetTargetFPS(60);

    Sound bgMusic = LoadSound("C:\\Users\\Admin\\Downloads\\Sakura-Girl-Daisy-chosic.com_.mp3");
    
    initializegrid();
    
    // Game variables
    int pressedX, pressedY;
    Box *firstBox = NULL;
    Box *secondBox = NULL;
    int score = 0;
    int time;

    int x=initialwindow(screenWidth,screenHeight);

    if(x==1)
     time=120;
    else 
     time=60;
    
    const int timerDuration = time; //  seconds countdown
    double startTime = GetTime(); // Store the start time


    // Start of the game loop
    while (!WindowShouldClose()) {
          
        

        if (!IsSoundPlaying(bgMusic)) 
            PlaySound(bgMusic);
    
        // Update
        double currentTime = GetTime();
        int remainingTime = timerDuration - (int)(currentTime - startTime);

        // Check if a box is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            pressedX = GetMouseX() / BOX_SIZE;
            pressedY = GetMouseY() / BOX_SIZE;

            // If no box has been picked yet, pick the first box
            if (firstBox == NULL && grid[pressedX][pressedY].matched!=true)
            {
                firstBox = &grid[pressedX][pressedY];
                firstBox->picked = true;
                firstBox->color = originalcolors[pressedX][pressedY].color;
            }

            // If one box has been picked, pick the second box
            else if (secondBox == NULL && firstBox != &grid[pressedX][pressedY] 
                    && grid[pressedX][pressedY].matched!=true) 
            {
                secondBox = &grid[pressedX][pressedY];
                secondBox->picked = true;
                secondBox->color = originalcolors[pressedX][pressedY].color;

                // If the colors of the two boxes match, they are matched
                if (firstBox->color.r == secondBox->color.r &&
                    firstBox->color.g == secondBox->color.g &&
                    firstBox->color.b == secondBox->color.b &&
                    firstBox->color.a == secondBox->color.a)
                {
                    firstBox->matched = true;
                    secondBox->matched = true;
                    score++; 
                } 
                else 
                {
                    // If the colors don't match, hide them again after a delay
                    for (int i = 0; i < 20; i++) 
                    {
                        if (WindowShouldClose())
                            return 0;
                        BeginDrawing();
                        ClearBackground(RAYWHITE);

                        // Draw all boxes
                        for (int i = 0; i < GRID_SIZE; i++){
                            for (int j = 0; j < GRID_SIZE; j++){
                                DrawRectangleRec(grid[i][j].rec, grid[i][j].color);
                                DrawRectangleLines(i * BOX_SIZE, j * BOX_SIZE, BOX_SIZE, BOX_SIZE, BLACK); }}
                        EndDrawing();
                    }

                    // Hide the boxes again
                    firstBox->picked = false;
                    firstBox->color = BLUE;
                    secondBox->picked = false;
                    secondBox->color = BLUE;
                }

                // Reset the picked boxes
                firstBox = NULL;
                secondBox = NULL;
            }
        }

        // Draw the remaining time below the score
        int minutes = remainingTime / 60;
        int seconds = remainingTime % 60;

        // If time is up, show the "TRY AGAIN!" message
        if (remainingTime <= 0 || score==8) 
        {
            // Clear the game screen
            StopSound(bgMusic);
            BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Create rectangles for "Restart" and "Quit" options
            Rectangle restartRect = {screenWidth/2 - 100, screenHeight/2 - 50, 200, 50};
            Rectangle quitRect = {screenWidth/2 - 100, screenHeight/2 + 50, 200, 50};

            // Check if the mouse button is pressed and if the mouse position is within either of the rectangles
            Vector2 mousePos = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePos, restartRect)) {
                    // Resetting game code

                    // Reset the score
                    score = 0;

                    // Reset the timer
                    startTime = GetTime();

                    // Reset the boxes
                    for (int i = 0; i < GRID_SIZE; i++) {
                        for (int j = 0; j < GRID_SIZE; j++) {
                            grid[i][j].picked = false;
                            grid[i][j].matched = false;
                            grid[i][j].color = BLUE;
                            originalcolors[i][j].selected = false;
                        }
                    }

                    // Re-initialize the grid
                    initializegrid();
                } 
                else if (CheckCollisionPointRec(mousePos, quitRect)) {
                    // Close the window 
                    CloseWindow();
                    return 0;
                }
            }

            // Draw the "Restart" and "Quit" rectangles
            DrawRectangleRec(restartRect, LIGHTGRAY);
            DrawRectangleRec(quitRect, LIGHTGRAY);

            // Display the "You Won" or "You Lose" message
            if (score == 18) {
                DrawText("YOU WON!", screenWidth/2 - MeasureText("YOU WON!", 30)/2, screenHeight/2 - 100, 30, RED);
            } else {
                DrawText("YOU LOSE!", screenWidth/2 - MeasureText("YOU LOSE!", 30)/2, screenHeight/2 - 100, 30, RED);
            }

            // Display the "RESTART" and "QUIT" messages
            DrawText("RESTART", restartRect.x + restartRect.width/2 - MeasureText("RESTART", 30)/2, restartRect.y + restartRect.height/2 - 30/2, 30, RED);
            DrawText("QUIT", quitRect.x + quitRect.width/2 - MeasureText("QUIT", 30)/2, quitRect.y + quitRect.height/2 - 30/2, 30, RED);

            EndDrawing();
        } 
        else
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Draw all boxes
            for (int i = 0; i < GRID_SIZE; i++) {
                for (int j = 0; j < GRID_SIZE; j++) {
                    DrawRectangleRec(grid[i][j].rec, grid[i][j].color);
                    DrawRectangleLines(i * BOX_SIZE, j * BOX_SIZE, BOX_SIZE, BOX_SIZE, BLACK);
                }
            }

            // Draw the score
            DrawText(TextFormat("Score:"), screenWidth - 220, 50, 45, DARKBLUE);
            DrawText(TextFormat("%d",score), screenWidth - 170, 100, 40, BLUE);

            DrawText(TextFormat("Time Left:"), screenWidth - 220, 180, 35, BLACK);
            DrawText(TextFormat("%02d:%02d", minutes, seconds), screenWidth - 180, 230, 30, MAROON);

            EndDrawing();
        }
    }

    CloseWindow();
    UnloadSound(bgMusic);
    return 0;
}


//display Initial window
int  initialwindow(int screenWidth,int screenHeight)
 {
    // Create rectangles for "Easy" and "Tough" options
    Rectangle easyRect = {screenWidth/2 - 100, screenHeight/2 - 50, 200, 50};
    Rectangle toughRect = {screenWidth/2 - 100, screenHeight/2 + 50, 200, 50};

    int level = 0;

    while (level == 0) {

        // Check if the mouse button is pressed and if the mouse position is within either of the rectangles
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, easyRect)) {
                 level =1;
                } else if (CheckCollisionPointRec(mousePos, toughRect)) {
                  level=2;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the "Easy" and "Tough" rectangles
        DrawRectangleRec(easyRect, LIGHTGRAY);
        DrawRectangleRec(toughRect, LIGHTGRAY);

        // Display the "EASY" and "TOUGH" messages
          DrawText("EASY", easyRect.x + easyRect.width/2 - MeasureText("EASY", 30)/2, easyRect.y + easyRect.height/2 - 30/2, 30, RED);
        DrawText("TOUGH", toughRect.x + toughRect.width/2 - MeasureText("TOUGH", 30)/2, toughRect.y + toughRect.height/2 - 30/2, 30, RED);

        EndDrawing();
 }
 return level;
 }
 

//initializing grids
void initializegrid()
{
    // Initialize selected cards and game grid
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            originalcolors[i][j].selected = false;
            grid[i][j].picked = false;
            grid[i][j].matched = false;
            grid[i][j].rec = (Rectangle){i * BOX_SIZE, j * BOX_SIZE, BOX_SIZE, BOX_SIZE};
            grid[i][j].color = BLUE;
        }
    }

    // Initialize unique colors in array uniquecolors
    for (int i = 0; i < GRID_SIZE * GRID_SIZE / 2; i++) {
        uniquecolors[i].color = getcolor(i);
    }

    // Fill in the originalcolors array
    for (int i = 0; i < GRID_SIZE * GRID_SIZE / 2; i++) {
        int index1, index2, index3, index4;
        do {
            index1 = rand() % GRID_SIZE;
            index2 = rand() % GRID_SIZE;
        } while (originalcolors[index1][index2].selected);

        originalcolors[index1][index2].color = uniquecolors[i].color;
        originalcolors[index1][index2].selected = true;

        do {
            index3 = rand() % GRID_SIZE;
            index4 = rand() % GRID_SIZE;
        } while (originalcolors[index3][index4].selected || (index1 == index3 && index2 == index4));

        originalcolors[index3][index4].color = uniquecolors[i].color;
        originalcolors[index3][index4].selected = true;
    }
}

// Get color function
Color getcolor(int i) {
    switch(i) {
        case 0: return  RAYWHITE;
        case 1: return (Color){255, 0, 0, 100}; // RED
        case 2: return DARKGRAY;
        case 3: return MAROON;
        case 4: return ORANGE;
        case 5: return DARKGREEN;
        case 6: return PURPLE;
        case 7: return (Color){100, 130, 138, 250}; // GOLD
        case 8: return YELLOW;
        case 9: return DARKBLUE;
        case 10: return DARKBROWN;
        case 11: return (Color){100, 130, 238, 255}; // VIOLET
        case 12: return PINK;
        case 13: return LIGHTGRAY;
        case 14: return DARKPURPLE;
        case 15: return BEIGE;
        case 16: return SKYBLUE;
        case 17: return GREEN;
    }
    return BLACK;
}