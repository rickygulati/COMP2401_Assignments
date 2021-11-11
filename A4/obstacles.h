typedef struct{
  int x;
  int y;
  int w;
  int h;
}Obstacle;

typedef struct{
  int x;
  int y;
  struct Neighbour *firstNeighbour;
  struct Neighbour *lastNeighbour;
  Obstacle *o;
}Vertex;

struct Neighbour{
  Vertex *vertex;
  struct Neighbour *next;
};

typedef struct{
  Obstacle *obstacles;
  int numObstacles;
  int numVertices;
  int numEdges;
  Vertex *vertices;
}Environment;

typedef struct Neighbour Neighbour;

int createVertices(Environment *environment);
int createEdges(Environment *environment);
int removeEdges(Environment *environment);
void cleanupEverything(Environment *environment);
