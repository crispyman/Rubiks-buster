#include "HELPERS.h"

//  cube    - a pointer to the cube.
//  side    - which side we want to rotate.
//  cc      - whether the rotation is counter clockwise or not.
void rotate(cube_t* cube, side_t side, int cc) {
  
}

// Rotate a sub cube.
void rotate_sub_cube(color_t* sub_cube, rotate_t rotation) {

  color_t front_color = sub_cube[FRONT];;

  switch (rotation) {
    case R_UP: {
      sub_cube[FRONT] = sub_cube[BOTTOM];
      sub_cube[BOTTOM] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[TOP];
      sub_cube[TOP] = front_color;
      break;
    }
    case R_DOWN: {
      sub_cube[FRONT] = sub_cube[TOP];
      sub_cube[TOP] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[BOTTOM];
      sub_cube[BOTTOM] = front_color;
      break;
    }
    case R_LEFT: {
      sub_cube[FRONT] = sub_cube[RIGHT];
      sub_cube[RIGHT] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[LEFT];
      sub_cube[LEFT] = front_color;
      break;
    }
    case R_RIGHT: {
      sub_cube[FRONT] = sub_cube[LEFT];
      sub_cube[LEFT] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[RIGHT];
      sub_cube[RIGHT] = front_color;
      break;
    }
  }
}