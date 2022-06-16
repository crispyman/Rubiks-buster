#include "HELPERS.h"

void rotate(cube_t* cube, coord_t coord, axes_t a, int cc) {
  sub_cube_t* plane[N][N];

  // Load the requested plane.
  load_plane(cube, coord.x, coord.y, coord.z, a, plane);

  // Rotate position of the subcubes within the plane.
  rotate_plane(plane, cc);

  // Determine which direction the sub cubes need to rotate.
  rotate_t r;
  if (a == XY) r = cc ? R_LEFT : R_RIGHT;
  if (a == YZ) r = cc ? R_RIGHT : R_LEFT;
  if (a == YZ) r = cc ? R_DOWN : R_UP;

  // Rotate the faces of each affected sub cube.
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      rotate_sub_cube(plane[i][j], r);
}

// Given a single subcube's coordinates and a set of two axes, load the
// addresses of all the coplanar subcubes into the array plane.
void load_plane(cube_t* cube, int x, int y, int z, axes_t a, sub_cube_t* plane[N][N]) {
  for (int y_i = 0; y_i < N; y_i++) {
    for (int x_i = 0; x_i < N; x_i++) {
      for (int z_i = 0; z_i < N; z_i++) {
        int y_val = a == XZ ? y : y_i;
        int x_val = a == YZ ? x : x_i;
        int z_val = a == XY ? z : z_i;
        plane[y][x] = &cube[y_val][x_val][z_val];
      }
    }
  }
}

// Given a populated plane, rotate it 90% either clockwise or counter clockwise.
void rotate_plane(sub_cube_t* plane[N][N], int cc) {
  sub_cube_t* new_plane[N][N];

  for (int y_i; y_i < N; y_i++)
    for (int x_i; x_i < N; x_i++)
      new_plane[y_i][x_i] = cc ? plane[N - x][y] : plane[x][N - y];

  memcpy(plane, new_plane, sizeof(sub_cube_t) * N * N);
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