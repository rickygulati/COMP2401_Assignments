#include <stdio.h>
#include <stdlib.h>

#include "obstacles.h"

int createVertices(Environment *environment);
int createEdges(Environment *environment);
int removeEdges(Environment *environment);
int isIntersect(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4);
void cleanupEverything(Environment *environment);

int createVertices(Environment *environment){
  if(environment->numObstacles <= 0) {
    return 0;
  }
  environment->numVertices = 4 * environment->numObstacles;
  environment->vertices = (Vertex *) malloc(environment->numVertices * sizeof(Vertex));

  if(environment->vertices == NULL){
    printf("Malloc failed!\n");
    return 0;
  }
  int k = 0;
  for(int i = 0; i < environment->numObstacles; i++){
    environment->vertices[k].x = environment->obstacles[i].x; //Top left x
    environment->vertices[k].y = environment->obstacles[i].y; //Top left y
    environment->vertices[k].firstNeighbour = environment->vertices[k].lastNeighbour = NULL; //Top left firstNeighbour & lastNeighbour
    environment->vertices[k++].o = &(environment->obstacles[i]);

    environment->vertices[k].x = environment->obstacles[i].x + environment->obstacles[i].w; //Top right x
    environment->vertices[k].y = environment->obstacles[i].y; //Top right y
    environment->vertices[k].firstNeighbour = environment->vertices[k].lastNeighbour = NULL; //Top left firstNeighbour & lastNeighbour
    environment->vertices[k++].o = &(environment->obstacles[i]);

    environment->vertices[k].x = environment->obstacles[i].x; //Bottom left x
    environment->vertices[k].y = environment->obstacles[i].y - environment->obstacles[i].h; //Bottom left y
    environment->vertices[k].firstNeighbour = environment->vertices[k].lastNeighbour = NULL; //Top left firstNeighbour & lastNeighbour
    environment->vertices[k++].o = &(environment->obstacles[i]);

    environment->vertices[k].x = environment->obstacles[i].x + environment->obstacles[i].w; //Bottom right x
    environment->vertices[k].y = environment->obstacles[i].y - environment->obstacles[i].h; //Bottom right y
    environment->vertices[k].firstNeighbour = environment->vertices[k].lastNeighbour = NULL; //Top left firstNeighbour & lastNeighbour
    environment->vertices[k++].o = &(environment->obstacles[i]);
  }
  return environment->numVertices;
}

int createEdges(Environment *environment){
  environment->numEdges = 0;

  for(int i = 0; i < environment->numVertices; i++){
    for(int j = 0; j < environment->numVertices; j++){
      if(i == j){
        continue;
      }
      Neighbour *newNeighbour = (Neighbour *) malloc(sizeof(Neighbour));
      if(newNeighbour == NULL){
        printf("Malloc failed\n");
        return 0;
      }
      newNeighbour->vertex = &(environment->vertices[j]);
      newNeighbour->next = environment->vertices[i].firstNeighbour;
      environment->vertices[i].firstNeighbour = newNeighbour;
      (environment->numEdges)++;
    }
  }
  return environment->numEdges;
}

int removeEdges(Environment *environment){
  int flag;

  for(int i = 0; i < environment->numVertices; i++){
    Neighbour *curr = environment->vertices[i].firstNeighbour;
    Neighbour *previous = NULL;
    while(curr != NULL){
      flag = 0;
      for(int j = 0; j < environment->numObstacles; j++){
          int x1, x2, x3, x4, y1, y2, y3, y4;
          // top edge of obstacle
          x1 = environment->vertices[i].x;
          y1 = environment->vertices[i].y;
          x2 = curr->vertex->x;
          y2 = curr->vertex->y;
          x3 = environment->obstacles[j].x;
          y3 = environment->obstacles[j].y;
          x4 = environment->obstacles[j].x + environment->obstacles[j].w;
          y4 = environment->obstacles[j].y;

          flag = isIntersect(x1, x2, x3, x4, y1, y2, y3, y4);
          if(flag)
            break;

          //Left edge of obstacle
          x3 = environment->obstacles[j].x;
          y3 = environment->obstacles[j].y;
          x4 = environment->obstacles[j].x;
          y4 = environment->obstacles[j].y - environment->obstacles[j].h;

          flag = isIntersect(x1, x2, x3, x4, y1, y2, y3, y4);
          if(flag)
            break;

          // Bottom edge of obstacle
          x3 = environment->obstacles[j].x;
          y3 = environment->obstacles[j].y - environment->obstacles[j].h;
          x4 = environment->obstacles[j].x + environment->obstacles[j].w;
          y4 = environment->obstacles[j].y - environment->obstacles[j].h;

          flag = isIntersect(x1, x2, x3, x4, y1, y2, y3, y4);
          if(flag)
            break;

          // Right edge of obstacle
          x3 = environment->obstacles[j].x + environment->obstacles[j].w;
          y3 = environment->obstacles[j].y;
          x4 = environment->obstacles[j].x + environment->obstacles[j].w;
          y4 = environment->obstacles[j].y - environment->obstacles[j].h;

          flag = isIntersect(x1, x2, x3, x4, y1, y2, y3, y4);
          if(flag)
            break;

          // For diagonal 1
          x4 = environment->obstacles[j].x + environment->obstacles[j].w;
          y4 = environment->obstacles[j].y;
          x3 = environment->obstacles[j].x;
          y3 = environment->obstacles[j].y - environment->obstacles[j].h;

          flag = isIntersect(x1, x2, x3, x4, y1, y2, y3, y4);
          if(flag)
            break;

            // For diagonal 2
            x4 = environment->obstacles[j].x;
            y4 = environment->obstacles[j].y;
            x3 = environment->obstacles[j].x + environment->obstacles[j].w;
            y3 = environment->obstacles[j].y - environment->obstacles[j].h;

            flag = isIntersect(x1, x2, x3, x4, y1, y2, y3, y4);
            if(flag)
              break;

        }
        if(!flag){
          previous = curr;
          curr = curr->next;
        }
        else{
          if(previous == NULL){ // which means its the first neighbour
            Neighbour *temp = environment->vertices[i].firstNeighbour; // So that we can save this value as we'll be freeing the curr pointer
            environment->vertices[i].firstNeighbour = curr->next;
            curr = curr->next;
            free(temp);
            (environment->numEdges)--;
          }
          else{
            Neighbour *temp = curr; // for saving value of curr
            previous->next = curr->next;
            curr = curr->next;
            free(temp);
            (environment->numEdges)--;
          }
        }

    }
  }
  return environment->numEdges;
}

int isIntersect(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4){
  float d = (y4-y3)*(x2-x1) - (x4-x3)*(y2-y1);
  if(d == 0)
    return 0;

  float num1 = (x4-x3)*(y1-y3) - (y4-y3)*(x1-x3);
  float num2 = (x2-x1)*(y1-y3) - (y2-y1)*(x1-x3);

  float ua = num1/d;
  float ub = num2/d;

  if((ua > 0) && (ua < 1) && (ub > 0) && (ub < 1)){
    return 1;
  }
  else{
    return 0;
  }
}

void cleanupEverything(Environment *environment){
  for(int i = 0; i < environment->numVertices; i++){
    Neighbour *curr = environment->vertices[i].firstNeighbour;
    while(curr != NULL){
      Neighbour *temp = curr;
      curr = curr->next;
      free(temp);
    }
  }
  free(environment->vertices);
  free(environment->obstacles);
}
