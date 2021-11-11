#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Defining Constants
#define MAX_BOXES 6
#define BOX_COLUMNS 4
#define BOX_ROWS 5
#define MAX_PACKAGES 24
#define LARGE_PRICE 19.99
#define MEDIUM_PRICE 10.99
#define SMALL_PRICE 5.99
#define BOX_PRICE 1.49

// Function Signatures
void assignLargePackages(char[][BOX_COLUMNS], int*);
void assignMediumPackages(char[][BOX_COLUMNS], int*);
void assignSmallPackages(char[][BOX_COLUMNS], int*);
void printOrder(char [][BOX_ROWS][BOX_COLUMNS], int, int, int, int);

// -----------------------------------------Body----------------------------------------
int main()
{
    srand(time(NULL));

    int largePackages, mediumPackages, smallPackages;
    char boxes[MAX_BOXES][BOX_ROWS][BOX_COLUMNS] = {0};

    // Generating random number of packages
    largePackages = rand() % (MAX_PACKAGES + 1);
    mediumPackages = rand() % (MAX_PACKAGES + 1 - largePackages);
    smallPackages = MAX_PACKAGES - largePackages - mediumPackages;
    int largePackagesCopy = largePackages, mediumPackagesCopy = mediumPackages, smallPackagesCopy = smallPackages;


    // Filling packages in boxes by calling the specific functions of each type of package
    int boxCount = 0;
    while(largePackages != 0 || mediumPackages != 0 || smallPackages != 0)
    {
        int largePackagesPlaced = largePackages; // keeps info about number of large packages placed
        int mediumPackagesPlaced = mediumPackages;// keeps info about number of medium packages placed
        assignLargePackages(boxes[boxCount], &largePackages);
        largePackagesPlaced -= largePackages;
        if((20 - largePackagesPlaced * 4) >= 2) // If after placing large packages in the box more than or equal to 2 places are left, we call the assignMediumPackages() function
        {
            assignMediumPackages(boxes[boxCount], &mediumPackages);
            mediumPackagesPlaced -= mediumPackages;
            if(20 - mediumPackagesPlaced * 2 >= 1) // If after placing medium packages in the box more than or equal to 1 places are left, we call the assignSmallPackages() function
            {
                assignSmallPackages(boxes[boxCount], &smallPackages);
            }
        }
        boxCount++;
    }

    printOrder(boxes, boxCount, largePackagesCopy, mediumPackagesCopy, smallPackagesCopy);
}

// --------------------------------------------Function Definitions---------------------------------------------------

// Assigns large packages to a single box
void assignLargePackages(char aBox[][BOX_COLUMNS], int *largePackets)
{
    if(*largePackets == 0)
        return;
    else
    {
        for(int i = 0; i < BOX_ROWS - 1; i++)
        {
            for(int j = 0; j < BOX_COLUMNS - 1; j++)
            {
                if(aBox[i][j] == 0 && aBox[i][j + 1] == 0 && aBox[i + 1][j] == 0 && aBox[i + 1][j + 1] == 0)
                {
                    if(*largePackets == 0)
                        return;

                    aBox[i][j] = aBox[i][j + 1] = aBox[i + 1][j] = aBox[i + 1][j + 1] = 'L';
                    --*(largePackets);
                }
            }
        }
    }

}

// Assigns medium packages to a single box
void assignMediumPackages(char aBox[][BOX_COLUMNS], int *mediumPackets)
{
    if(*mediumPackets == 0)
        return;
    else
    {
        for(int i = 0; i < BOX_ROWS; i++)
        {
            for(int j = 0; j < BOX_COLUMNS; j++)
            {
                if(aBox[i][j] == 0 && aBox[i][j + 1] == 0 && j != BOX_COLUMNS - 1)
                {
                    aBox[i][j] = aBox[i][j + 1] = 'M';
                    --*(mediumPackets);
                    if(*mediumPackets == 0)
                        return;
                }
                else if(aBox[i][j] == 0 && aBox[i + 1][j] == 0 && i != BOX_ROWS - 1)
                {
                    aBox[i][j] = aBox[i][j + 1] = 'M';
                    --*(mediumPackets);
                    if(*mediumPackets == 0)
                        return;
                }
            }
        }
    }
}

// Assigns small packages to single box
void assignSmallPackages(char aBox[][BOX_COLUMNS], int *smallPackets)
{
    if(*smallPackets == 0)
        return;
    else
    {
        for(int i = 0; i < BOX_ROWS; i++)
        {
            for(int j = 0; j < BOX_COLUMNS; j++)
            {
                if(aBox[i][j] == 0)
                {
                    aBox[i][j] = 'S';
                    --*(smallPackets);
                    if(*smallPackets == 0)
                        return;
                }
            }
        }
    }

}

// Prints the order summary and the arrangement of packages in the boxes
void printOrder(char allBoxes[][BOX_ROWS][BOX_COLUMNS], int boxesUsed, int large, int medium, int small)
{
    printf("Order:\tLarge = %d,\tMedium = %d,\tSmall = %d\n", large, medium, small);
    printf("%-20s(%-02d units)\t$ %6.2f\n", "Small ", small, small * SMALL_PRICE);
    printf("%-20s(%-02d units)\t$ %6.2f\n", "Medium ", medium, medium * MEDIUM_PRICE);
    printf("%-20s(%-02d units)\t$ %6.2f\n", "Large ", large, large * LARGE_PRICE);
    for(int i = 0; i <= 40; i++)
        printf("-");
    float totalPrice = (small * SMALL_PRICE) + (medium * MEDIUM_PRICE) + (large * LARGE_PRICE);
    printf("\n%-25s\t$ %6.2f", "Total ", totalPrice);
    printf("\n%-20s(%-2d boxes)\t$ %6.2f", "Shipping ", boxesUsed, boxesUsed * BOX_PRICE);
    printf("\nHST(13%s)%19s\t$ %6.2f\n", "%", " ", 0.13 * totalPrice);
    for(int i = 0; i <= 40; i++)
        printf("-");
    printf("\n%-32s$ %6.2f", "Amount Charged", 1.13 * totalPrice);

    printf("\n\nIndividual boxes are packed as follows:\n");

   for(int i = 0; i < boxesUsed; i++)
   {
       for(int j = 0; j < BOX_ROWS; j++)
       {
           printf("\n");
           for(int k = 0; k < BOX_COLUMNS; k++)
           {
               if(allBoxes[i][j][k] == 0)
                    printf("  ");
               else
                printf("%c ", allBoxes[i][j][k]);
           }
       }
       printf("\n\n");
   }
}
