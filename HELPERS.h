#ifndef RUBIKS_BUSTER_HELPERS_H
#define RUBIKS_BUSTER_HELPERS_H
   #define N 3

  typedef enum {NONE, WHITE, GREEN, RED, BLUE, ORANGE, YELLOW} color_t;
  typedef enum {TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM} side_t;
  typedef enum {R_UP, R_DOWN, R_LEFT, R_RIGHT} rotate_t;
  typedef color_t sub_cube_t[6];
  typedef sub_cube_t cube_t[N][N][N];

  // Prototypes
  void rotate(cube_t* cube, side_t side, int cc);
  void rotate_sub_cube(color_t* sub_cube, rotate_t rotation);

#endif //RUBIKS_BUSTER_HELPERS_H
