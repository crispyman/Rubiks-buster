#include "HELPERS.h"

//  cube    - a pointer to the cube.
//  side    - which side we want to rotate.
//  cc      - whether the rotation is counter clockwise or not.
void rotate(cube_t* cube, side_t side, int cc) {
  // Rotate the face of the side chosen.
  rotateFace((color_t*)(cube + (side * 8)), cc);

  // Rotate the edges of the faces.
  switch (side) {
    
  }
}

// Rotate a sub cube.
void rotate_sub_cube(color_t* sub_cube, rotate_t* rotation) {

  color_t front_color = sub_cube[FRONT];;

  switch (rotation) {
    case UP: {
      sub_cube[FRONT] = sub_cube[BOTTOM];
      sub_cube[BOTTOM] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[TOP];
      sub_cube[TOP] = front_color
      break;
    }
    case DOWN: {
      sub_cube[FRONT] = sub_cube[TOP];
      sub_cube[TOP] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[BOTTOM];
      sub_cube[BOTTOM] = front_color;
      break;
    }
    case LEFT: {
      sub_cube[FRONT] = sub_cube[RIGHT];
      sub_cube[RIGHT] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[LEFT];
      sub_cube[LEFT] = front_color;
      break;
    }
    case RIGHT: {
      sub_cube[FRONT] = sub_cube[LEFT];
      sub_cube[LEFT] = sub_cube[BACK];
      sub_cube[BACK] = sub_cube[RIGHT];
      sub_cube[RIGHT] = front_color;
      break;
    }
  }
}