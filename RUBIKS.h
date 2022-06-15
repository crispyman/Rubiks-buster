//
// Created by andrewiii on 6/15/22.
//

#define N 3

#ifndef RUBIKS_BUSTER_RUBIKS_H
#define RUBIKS_BUSTER_RUBIKS_H

  typedef enum {NONE, WHITE, GREEN, RED, BLUE, ORANGE, YELLOW} color_t;
  typedef enum {TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM} side_t;
  typedef color_t sub_cube_t[6];
  typedef sub_cube_t cube_t[N][N][N];

  // Prototypes
  void rotate(cube_t* cube, side_t side, int cc);
  void rotateFace(color_t* face, int cc);

#endif //RUBIKS_BUSTER_RUBIKS_H
