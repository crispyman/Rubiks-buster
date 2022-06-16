#include <stdbool.h>

#ifndef RUBIKS_BUSTER_HELPERS_H
#define RUBIKS_BUSTER_HELPERS_H
  #define N 3

  typedef enum {NONE, WHITE, GREEN, RED, BLUE, ORANGE, YELLOW} color_t;
  typedef enum {TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM} side_t;
  typedef enum {R_UP, R_DOWN, R_LEFT, R_RIGHT} rotate_t;
  typedef enum {XY, XZ, YZ} axes_t;
  typedef color_t sub_cube_t[6];
  typedef sub_cube_t cube_t[N][N][N];
  typedef struct {
          int x;
          int y;
          int z;
          axes_t a;
          int cc;
  } rotate_action_t;

  // Prototypes
  // void rotate(cube_t* cube, int x, int y, int z, axes_t a, int cc);
  void rotate(cube_t* cube, rotate_action_t act);
  void load_plane(cube_t* cube, rotate_action_t act, sub_cube_t* plane[N][N]);
  void rotate_plane(sub_cube_t* plane[N][N], int cc);
  void rotate_sub_cube(sub_cube_t sub_cube, rotate_t rotation);
  void verifyValid(cube_t* cube);
  bool checkSolved(cube_t* cube);

#endif //RUBIKS_BUSTER_HELPERS_H
