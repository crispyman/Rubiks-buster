#include <string.h>
#include "HELPERS.h"

 void rotate(cube_t* cube, rotate_action_t act) {
  // Create a 2D array of sub cube pointers.
  sub_cube_t* plane[N][N];

  // Load the requested plane.
  load_plane(cube, act, plane);

  // Rotate position of the sub cubes within the plane.
  rotate_plane(plane, act.cc);

  // Determine which direction the sub cubes need to rotate.
  rotate_t r;
  if (act.a == XY) r = act.cc ? R_LEFT : R_RIGHT;
  else if (act.a == YZ) r = act.cc ? R_RIGHT : R_LEFT;
  else if (act.a == YZ) r = act.cc ? R_DOWN : R_UP;
  else return;

  // Rotate the faces of each affected sub cube.
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      rotate_sub_cube(*plane[i][j], r);
}

// Given a single subcube's coordinates and a set of two axes, load the
// addresses of all the coplanar subcubes into the array plane.
void load_plane(cube_t* cube, rotate_action_t act, sub_cube_t* plane[N][N]) {
  for (int x_i = 0; x_i < N; x_i++) {
    for (int y_i = 0; y_i < N; y_i++) {
      for (int z_i = 0; z_i < N; z_i++) {
        int x_val = act.a == YZ ? act.x : x_i;
        int y_val = act.a == XZ ? act.y : y_i;
        int z_val = act.a == XY ? act.z : z_i;
        plane[x_i][y_i] = cube[x_val][y_val][z_val];
      }
    }
  }
}

// Given a populated plane, rotate it 90% either clockwise or counter clockwise.
void rotate_plane(sub_cube_t* plane[N][N], int cc) {
  sub_cube_t new_plane[N][N];

  for (int x_i; x_i < N; x_i++)
    for (int y_i; y_i < N; y_i++)
      memcpy(new_plane[x_i][y_i], cc ? *plane[y_i][N - x_i] : *plane[N - y_i][x_i], sizeof(sub_cube_t));

  for (int x_i; x_i < N; x_i++)
    for (int y_i; y_i < N; y_i++)
      memcpy(*plane[x_i][y_i], new_plane[x_i][y_i], sizeof(sub_cube_t));
}

// Rotate a sub cube.
void rotate_sub_cube(sub_cube_t sub_cube, rotate_t rotation) {

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