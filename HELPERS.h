#ifndef RUBIKS_BUSTER_HELPERS_H
#define RUBIKS_BUSTER_HELPERS_H
  #include <stdbool.h>

  #define SIDES 6
  #define N 3
  #define DEBUG 1

  typedef enum {NONE, WHITE, GREEN, RED, BLUE, ORANGE, YELLOW} color_t;
  typedef enum {TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM} side_t;
  typedef enum {R_UP, R_DOWN, R_LEFT, R_RIGHT} rotate_t;
  typedef enum {XY, XZ, YZ} axes_t;
  typedef color_t cube_t[SIDES][N][N];
  typedef struct {
      axes_t a;
      int index;
      int cc;
  } rotate_action_t;

  // the side_t enum was in a weird order, so I made these Cycles to use
  const static side_t cycleX[] = {FRONT, RIGHT, BACK, LEFT};
  const static side_t cycleY[] = {FRONT, TOP, BACK, BOTTOM};
  const static side_t cycleZ[] = {RIGHT, BOTTOM, LEFT, TOP};



  // Prototypes
  // void rotate(cube_t* cube, int x, int y, int z, axes_t a, int cc);
  // Borrowed from Dr. Norris, Thank You!
  void unixError(char * msg);

  void rotate(cube_t* cube, rotate_action_t act);
  void rotateX(cube_t* cube,int row, int cc);
  void rotateY(cube_t* cube,int col, int cc);
  void rotateZ(cube_t* cube,int col, int cc);

  void rotate_face(cube_t* cube, int side, int cc);

  void verifyValid(cube_t* cube);
  bool checkSolved(cube_t* cube);

#endif //RUBIKS_BUSTER_HELPERS_H
