//
// Created by andrewiii on 6/15/22.
//

#ifndef RUBIKS_BUSTER_RUBIKS_H
#define RUBIKS_BUSTER_RUBIKS_H

  enum Color {WHITE, GREEN, RED, BLUE, ORANGE, YELLOW};
  typedef Color color_t;

  enum Side {TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM};
  typedef Side side_t;

  typedef color_t[48] cube_t;

  cube_t Cube;

  // Prototypes

#endif //RUBIKS_BUSTER_RUBIKS_H
