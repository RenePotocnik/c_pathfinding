#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

// Define the maze properties - can be changed to support other maze types
#define HEIGHT 93
#define WIDTH 93
#define START 'S'
#define FREE '.'
#define WALL '#'
#define END 'E'

int findShortestPath(char grid[HEIGHT][WIDTH], bool visited[HEIGHT][WIDTH], int sx, int sy, int ex, int ey, int minDist,
                     int dist);

bool isFree(char grid[HEIGHT][WIDTH], bool visited[HEIGHT][WIDTH], int sx, int sy);

bool visited[HEIGHT][WIDTH];


int main() {
    int sx = -1, sy = -1;
    int ex = -1, ey = -1;
    int minDist;
    char grid[HEIGHT][WIDTH];

    FILE *maze;
    char fileName[50];
    char txt[] = {".txt"};

    // Prompt user to enter the file name of the maze
    printf("Current maze dimensions: %dx%d\nEnter maze file directory:\n>", HEIGHT, WIDTH);
    scanf("%s", fileName);
    // Append `.txt` to the end
//    strcat(fileName, txt);

    maze = fopen(fileName, "r");
    if (!maze) {
        // The file was not opened/found
        printf("ERROR: Incorrect file name: '%s'", fileName);
        return -1;
    }

    // Read the maze in the file and write it into `grid`
    for (int i = 0; i < HEIGHT; i++) {
        fgets(grid[i], WIDTH + 1, maze);
    }


    // Calculate the size of the file to determine the size of the maze (assuming the maze is square)
    fseek(maze, 0L, SEEK_END);
    long int size = ftell(maze);
    int mazeDimension = (int) sqrt(size);
    if (mazeDimension != HEIGHT) {
        printf("ERROR! The Dimensions of the maze in the entered file don't match the set dimensions (%dx%d)\n"
               "Set the maze dimension to `%d` if you want to use %s",
               HEIGHT, HEIGHT, mazeDimension, fileName);
        return -1;
    }
    fclose(maze);


    // Find `START` and `END`
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == START) {
                sx = i;
                sy = j;
            } else if (grid[i][j] == END) {
                ex = i;
                ey = j;
            }
        }
    }

    // Print the grid
    printf("Grid:\n");
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }

    if (sx < 0) {
        printf("The start point could not be found");
        return -1;
    } else if (ex < 0) {
        printf("The end point could not be found");
        return -1;
    } else {
        printf("Start: %d*%d\tEnd: %d*%d\n", sx, sy, ex, ey);
    }

    minDist = findShortestPath(grid, visited, sx, sy, ex, ey, INT_MAX, 0);
    if (minDist != INT_MAX) {
        printf("Minimum distance: %d\n", minDist);
    }
/*
    // Print all visited cells
    printf("Visited Grid:\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (visited[i][j] == true) {
                printf("\033[1m%c\033[0m", grid[i][j]);
            }
            else {
                printf("\033[30m%c\033[0m", grid[i][j]);
            }
        }
        printf("\n");
    }
*/
    return 0;
}

int findShortestPath(char grid[HEIGHT][WIDTH], bool visited[HEIGHT][WIDTH], int sx, int sy, int ex, int ey, int minDist,
                     int dist) {
    if (sx == ex && sy == ey) {
        // If the start point is the end point
        return (minDist < dist) ? minDist : dist;
    }

    visited[sx][sy] = true;
    if (isFree(grid, visited, sx + 1, sy)) {
        // Move down
        minDist = findShortestPath(grid, visited, sx + 1, sy, ex, ey, minDist, dist + 1);
    }
    if (isFree(grid, visited, sx, sy + 1)) {
        // Move right
        minDist = findShortestPath(grid, visited, sx, sy + 1, ex, ey, minDist, dist + 1);
    }
    if (isFree(grid, visited, sx - 1, sy)) {
        // Move up
        minDist = findShortestPath(grid, visited, sx - 1, sy, ex, ey, minDist, dist + 1);
    }
    if (isFree(grid, visited, sx, sy - 1)) {
        // Move left
        minDist = findShortestPath(grid, visited, sx, sy - 1, ex, ey, minDist, dist + 1);
    }
    // visited[sx][sy] = false;
    return minDist;
}


bool isFree(char grid[HEIGHT][WIDTH], bool visited[HEIGHT][WIDTH], int sx, int sy) {
    // Check if the cell is available to travel to
    //printf("%d*%d\n", sx, sy);
    return (sx >= 0 && sx < HEIGHT && sy >= 0 && sy < WIDTH) && (grid[sx][sy] == FREE || grid[sx][sy] == END) &&
           !visited[sx][sy];
}