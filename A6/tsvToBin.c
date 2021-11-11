#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUNTRY_LENGTH 25
#define MAX_LINE_LENGTH 183

int converter(char *);

int main(){
    FILE *fd = fopen("pandemic.tsv", "r");
    if(!fd){
        printf("ERROR: could not open file!\n");
        exit(1);
    }

    FILE *fOngoing;
    FILE *fResolved;
    FILE *fIncomplete;

    if((fOngoing = fopen("ongoing.bin", "w")) == NULL){
        printf("ERROR: could not create file!\n");
        exit(1);
    }

    if((fResolved = fopen("resolved.bin", "w")) == NULL){
        printf("ERROR: could not create file!\n");
        exit(1);
    }

    if((fIncomplete = fopen("incomplete.bin", "w")) == NULL){
        printf("ERROR: could not create file!\n");
        exit(1);
    }

    int totalLines = 0;
    char c;
     while(!feof(fd)){
        fscanf(fd, "%c", &c);
        if(c == '\n')
            totalLines++;
    }

    fseek(fd, 0, SEEK_SET);

    //ongoing : active cases
    //resolved : total cases = death + recovered
    //incomplete : missing total cases, total deaths, total recovered or population
    unsigned short int ongoingCount = 0, resolvedCount = 0,
                       incompleteCount = 0;

    char countryBuffer[MAX_COUNTRY_LENGTH];
    char lineBuffer[MAX_LINE_LENGTH];
    char numberBuffer[20];
    char emptyCheck;
    unsigned int totalCases, totalDeaths, totalRecovered, population;
    int activeCases;
    fscanf(fd, "%[^\n]s", lineBuffer);
    printf("%s", lineBuffer);
    c = fgetc(fd);
    //now on second line
    while(!feof(fd)){
      //reading rank
      fscanf(fd, "%[^\t]s", lineBuffer);
      fscanf(fd, "\t");
      printf("%s is rank\n", lineBuffer);
      //reading country
      fscanf(fd, "%[^\t]s", countryBuffer);
      fscanf(fd, "\t");
      printf("%s is country\n", countryBuffer);
      //reading total cases
      fscanf(fd, "%[^\t]s", numberBuffer);
      totalCases = converter(numberBuffer);
      printf("%d is total cases\n", totalCases);
      c = fgetc(fd);
      //skipping new cases but first check if it's empty or not
      //fscanf(fd, "%c", &c);
      c = fgetc(fd);
      if(c != '\t'){
        fseek(fd, -1, SEEK_CUR);
        fscanf(fd, "%[^\t]s", lineBuffer);
        printf("%s is new cases\n", lineBuffer);
        c = fgetc(fd);
      }
      else
        printf("New cases empty\n");
      //now reading total deaths but first check if it's empty
      c = fgetc(fd);
      if(c != '\t'){
        fseek(fd, -1, SEEK_CUR);
        fscanf(fd, "%[^\t]s", numberBuffer);
        totalDeaths = converter(numberBuffer);
        printf("%d is total deaths\n", totalDeaths);
        c = fgetc(fd);
      }
      else{
        totalDeaths = -1;
        printf("Total deaths empty\n");
      }
      //skipping new deaths but first check if it's empty

      c = fgetc(fd);
      if(c != '\t'){
        fseek(fd, -1, SEEK_CUR);
        fscanf(fd, "%[^\t]s", lineBuffer);
        printf("%s is new deaths and length of new deaths = %ld\n", lineBuffer, strlen(lineBuffer));
        c = fgetc(fd);
      }else
        printf("New deaths empty\n");

      //reading total recovered but first check if it's empty
      c = fgetc(fd);
      if(c != '\t'){
        fseek(fd, -sizeof(char), SEEK_CUR);
        fscanf(fd, "%[^\t]s", numberBuffer);
        totalRecovered = converter(numberBuffer);
        printf("%d is total recovered\n", totalRecovered);
        c = fgetc(fd);
      }
      else{
        totalRecovered = -1;
        printf("totalRecovered empty\n");
      }

      //now reading active cases
      fscanf(fd, "%[^\t]s", numberBuffer);
      activeCases = converter(numberBuffer);
      printf("%d is active cases\n", activeCases);
      c = fgetc(fd);
      // skipping 5 columns but first check if it's empty
      for(int i = 1; i <= 5; i++){
        c = fgetc(fd);
        if(c != '\t'){
          fseek(fd, -1, SEEK_CUR);

          fscanf(fd, "%[^\t]s", lineBuffer);
          printf("%s skipping\n", lineBuffer);
          c = fgetc(fd);
        }
        else
          printf("Skipping column empty\n");
      }
      //reading population
      c = fgetc(fd);
      if(c != '\t'){
        fseek(fd, -1, SEEK_CUR);
        fscanf(fd, "%[^\n]s", numberBuffer);
        population = (unsigned int)converter(numberBuffer);
        printf("population = %d\n", population);
      }
      else{
        population = -1;
        printf("population empty\n");
      }

      fscanf(fd, "\n");
      //end of line
      //now writing to FILE
      if(activeCases > 0){
        if(totalDeaths >= 0){
          if(totalRecovered >= 0){
            if(ongoingCount == 0){
              ongoingCount++;
              fwrite(&ongoingCount, sizeof(short int), 1, fOngoing);
            }
            else{
              long int pos = ftell(fOngoing);
              fseek(fOngoing, 0, SEEK_SET);
              ongoingCount++;
              fwrite(&ongoingCount, sizeof(short int), 1, fOngoing);
              fseek(fOngoing, pos, SEEK_SET);
            }
            unsigned char len = strlen(countryBuffer);
            fwrite(&len, sizeof(char), 1, fOngoing);
            fwrite(countryBuffer, sizeof(char), strlen(countryBuffer), fOngoing);
            char t = '\t', newline = '\n';
            int missing = -1;
            //fwrite(&t, sizeof(char), 1, fOngoing);
            fwrite(&totalCases, sizeof(int), 1, fOngoing);
            //fwrite(&t, sizeof(char), 1, fOngoing);
            fwrite(&totalDeaths, sizeof(int), 1, fOngoing);
            //fwrite(&t, sizeof(char), 1, fOngoing);
            fwrite(&totalRecovered, sizeof(int), 1, fOngoing);
            //fwrite(&t, sizeof(char), 1, fOngoing);
            if(population > 0){
              fwrite(&population, sizeof(int), 1, fOngoing);
            }
            else{
              fwrite(&missing, sizeof(int), 1, fOngoing);
            }
            fwrite(&newline, sizeof(char), 1, fOngoing);
          }
          else{

          }
        }
      }
    }
    printf("Ongoing count = %d\n", ongoingCount);
    fclose(fd);
    fclose(fOngoing);
    fclose(fResolved);
    fclose(fIncomplete);


}


int converter(char *s){
    if(s == "\t" || s == "" || strcmp(s, "N/A") == 0 || s == "\r" || s == "\n" || strlen(s) == 0 || s == " "){
        return -1;
    }
    else{
        char *temp = (char *) malloc(sizeof(char)*strlen(s));
        for(int i = 0, j = 0; i < strlen(s); i++){
            if((int)s[i] >= 48 && (int)s[i] <= 57)
                temp[j++] = s[i];
            }
        int n = atoi(temp);
        free(temp);
    return n;
    }

}
