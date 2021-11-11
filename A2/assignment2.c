#include <stdio.h>

#define MAZE_SIZE_32 32
#define MAZE_SIZE_8 8
#define MAZE_SIZE_14 14

// Function prototypes
void printEncodedMaze(int [], unsigned char);
void printEncodedMazeWithPath(int [], int [], unsigned char);
void encode8by8Maze(char [8][8], int []);
void merge8by8Mazes(int [8], int [8], int [8], int [8], int [14]);
char isPathInMaze(int[], int[], unsigned char);

int main()
{
    // 32 x 32 maze declaration and initialization
    int maze32[MAZE_SIZE_32] = {-1, -2146430423, -1109928277, -1525661045, 
                      -1602496045, -1358950569, -1451509759, 
                      -1927115297, -1578180479, -1354977603, 
                      -1476294999, -2084818261, -1163749757, 
                      -1964319689, -1341614619, -1750141691, 
                      -1256060947, -1515522751, -1204136611, 
                      -1955867371, -1190652827, -1553272339, 
                      -1100839163, -1999963019, -631368865, 
                      -1882138047, -1172944163, -1412279803, 
                      -1567107339, -164346691, -2138762879, 
                      -1};
    
    int path32[MAZE_SIZE_32] = {0, 0, 0, 0, 12, 8, 56, 32, 8032, 4416, 134115648, 67354944, 67109184,
                      67109312, 133169152, 1048576, 1835008, 262144, 262144, 262144, 458752,
                      65536, 65536, 65536, 65536, 983040, 67633152, 67633152, 201850880,
                      164102144, 259522560, 0};
    
    printf("\nPrinting 32 x 32 maze\n");
    printEncodedMaze(maze32, MAZE_SIZE_32);

    if(isPathInMaze(maze32, path32, MAZE_SIZE_32) == 1)
        printf("\nPath is there for 32 x 32 maze\n");
    else
        printf("\nPath is not there for 32 x 32 maze\n");
    
    printf("\nPrinting 32 x 32 maze with path\n");
    printEncodedMazeWithPath(maze32, path32, MAZE_SIZE_32);
    printf("\n\n");
    
    char maze1[MAZE_SIZE_8][MAZE_SIZE_8] = {{1,1,1,1,1,1,1,1},
                                            {1,0,0,0,1,0,0,1},
                                            {1,0,1,0,1,1,0,1},
                                            {1,0,1,0,0,0,0,1},
                                            {1,0,1,1,1,1,0,1},
                                            {1,0,0,0,0,0,0,1},
                                            {1,0,1,0,1,0,1,1},
                                            {1,1,1,1,1,1,1,1}};
    
    char maze2[MAZE_SIZE_8][MAZE_SIZE_8] = {{1,1,1,1,1,1,1,1},
                                            {1,0,0,0,0,1,0,1},
                                            {1,1,1,1,0,1,0,1},
                                            {1,0,0,1,0,1,1,1},
                                            {1,1,0,0,0,0,0,1},
                                            {1,1,1,1,0,1,1,1},
                                            {1,0,0,0,0,1,0,1},
                                            {1,1,1,1,1,1,1,1}};

    char maze3[MAZE_SIZE_8][MAZE_SIZE_8] = {{1,1,1,1,1,1,1,1},
                                            {1,0,1,0,0,0,1,1},
                                            {1,0,1,0,1,0,0,1},
                                            {1,0,1,0,1,0,1,1},
                                            {1,0,1,0,1,0,1,1},
                                            {1,0,1,0,1,0,1,1},
                                            {1,0,0,0,1,0,0,1},
                                            {1,1,1,1,1,1,1,1}};

    char maze4[MAZE_SIZE_8][MAZE_SIZE_8] = {{1,1,1,1,1,1,1,1},
                                            {1,0,1,0,1,0,1,1},
                                            {1,0,1,0,0,0,0,1},
                                            {1,0,0,0,1,1,1,1},
                                            {1,1,1,0,1,0,0,1},
                                            {1,0,0,0,0,0,1,1},
                                            {1,1,0,1,1,0,0,1},
                                            {1,1,1,1,1,1,1,1}};

    int encodedMaze1[MAZE_SIZE_8] = {0};
    int encodedMaze2[MAZE_SIZE_8] = {0};
    int encodedMaze3[MAZE_SIZE_8] = {0};
    int encodedMaze4[MAZE_SIZE_8] = {0};

    // Encoding four 8 x 8 mazes
    encode8by8Maze(maze1, encodedMaze1);
    encode8by8Maze(maze2, encodedMaze2);
    encode8by8Maze(maze3, encodedMaze3);
    encode8by8Maze(maze4, encodedMaze4);

    // Printing four 8 x 8 mazes
    printEncodedMaze(encodedMaze1, MAZE_SIZE_8);
    printf("\n\n");
    printEncodedMaze(encodedMaze2, MAZE_SIZE_8);
    printf("\n\n");
    printEncodedMaze(encodedMaze3, MAZE_SIZE_8);
    printf("\n\n");
    printEncodedMaze(encodedMaze4, MAZE_SIZE_8);
    printf("\n\n");

    int *mazePtrArr[] = {encodedMaze1, encodedMaze2, encodedMaze3, encodedMaze4};

    int path14[] = {0, 4096, 4096, 4096, 4096, 7936, 256, 256, 448, 112, 16, 28, 6, 0};

    int count = 0;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                for(int y = 0; y < 4; y++)
                {
                    int mergedMaze[MAZE_SIZE_14] = {0};
                    merge8by8Mazes(mazePtrArr[i], mazePtrArr[j], mazePtrArr[k], mazePtrArr[y], mergedMaze);
                    printf("\n\nPrinting merged maze with code: %d, %d, %d, %d\n\n", i + 1, j + 1, k + 1, y + 1);
                    if(isPathInMaze(mergedMaze, path14, MAZE_SIZE_14) == 1)
                        {
                            printf("\nPath is IN maze!\n");
                            printEncodedMazeWithPath(mergedMaze, path14, MAZE_SIZE_14);
                            count++;
                        }
                    else
                    {
                        printf("\nPath is NOT IN maze!\n");
                        printEncodedMaze(mergedMaze, MAZE_SIZE_14);
                    }
                }
            }
        }
    }
    printf("\nNumber of mazes with paths: %d\n", count);

}

// Function to print encoded maze
void printEncodedMaze(int maze[], unsigned char size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = size - 1; j >= 0; j--)
        {
            if((maze[i] & (1<<j)) != 0)
                printf("@");
            else
                printf(" ");
        }
        printf("\n");
    }
}

// Function to print encoded maze with path
void printEncodedMazeWithPath(int maze[], int path[], unsigned char size)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = size - 1; j >= 0; j--)
        {
            if((maze[i] & (1<<j)) != 0)
                printf("@");
            else if((path[i] & (1<<j)) != 0)
                printf(".");
            else 
                printf(" ");
        }
        printf("\n");
    }
}

// Function to encode 8 x 8 char maze to int [8] maze
void encode8by8Maze(char inputMaze[MAZE_SIZE_8][MAZE_SIZE_8], int encodedMaze[MAZE_SIZE_8])
{
    for(int i = 0; i < MAZE_SIZE_8; i++)
    {
        for(int j = MAZE_SIZE_8 - 1; j >= 0; j--)
        {
            if(inputMaze[i][MAZE_SIZE_8 - 1 - j] == 1)
                encodedMaze[i] = encodedMaze[i] | (1<<j);
        }
    }
}

// Function to merge four 8 x 8 mazes into 14 x 14 maze
void merge8by8Mazes(int topLeft[8], int topRight[8], int bottomLeft[8], int bottomRight[8], int finalMaze[14])
{
    for(int i = 0; i < MAZE_SIZE_14; i++)
    {
        for(int j = MAZE_SIZE_14 - 1; j >= 0; j--)
        {
            if( i < MAZE_SIZE_8 - 1)
            {
                if(j >= 7)
                    {
                        if((topLeft[i] & (1<<(j - (MAZE_SIZE_8 - 2)))) != 0)
                            {
                                finalMaze[i] = finalMaze[i] | (1<<j);
                                continue;
                            }
                    }
                else
                    {
                        if((topRight[i] & (1<<j)) != 0)
                            {
                                finalMaze[i] = finalMaze[i] | (1<<j);
                                continue;
                            }
                    }
            }
            else
            {
                if(j >= 7)
                    {
                        if((bottomLeft[i - (MAZE_SIZE_8 - 2)] & (1<<(j - (MAZE_SIZE_8 - 2)))) != 0)
                            {
                                finalMaze[i] = finalMaze[i] | (1<<j);
                                continue;
                            }
                    }
                else
                {
                    if((bottomRight[i - (MAZE_SIZE_8 - 2)] & (1<<j)) != 0)
                        {
                            finalMaze[i] = finalMaze[i] | (1<<j);
                            continue;
                        }
                }
                
            }
            
        }
    }
}

// Function to print encoded maze with path
char isPathInMaze(int maze[], int path[], unsigned char size)
{
    for(int i = 0; i < size; i++)
    {
        for( int j = size - 1; j >= 0; j--)
        {
            if(((path[i] & (1<<j)) != 0) && ((maze[i] & (1<<j)) != 0))
                return 0;
        }
    }
    return 1;
}