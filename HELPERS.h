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
  //typedef color_t sub_cube_t[6][N][N];
  typedef color_t cube_t[SIDES][N][N];
  typedef struct {
      axes_t a;
      int index;
      int cc;
  } rotate_action_t;

  const static side_t cycleX[] = {FRONT, RIGHT, BACK, LEFT};
  const static side_t cycleY[] = {RIGHT, BOTTOM, LEFT, TOP};
  const static side_t cycleZ[] = {FRONT, TOP, BACK, BOTTOM};



// Prototypes
  // void rotate(cube_t* cube, int x, int y, int z, axes_t a, int cc);
    void rotate(cube_t* cube, rotate_action_t act);
    void rotateX(cube_t* cube,int row, int cc);
    void rotateY(cube_t* cube,int row, int cc);
    void rotateZ(cube_t* cube,int row, int cc);

  void rotate_face(cube_t* cube, int side, int cc);

    void verifyValid(cube_t* cube);
  bool checkSolved(cube_t* cube);

#endif //RUBIKS_BUSTER_HELPERS_H
