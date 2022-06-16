#include <string.h>
#include <stdbool.h>
#include <assert.h>
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
void verifyValid(cube_t* cube) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (i == 0)
                    assert((*cube)[i][j][k][TOP] != NONE);
                else
                    assert((*cube)[i][j][k][TOP] == NONE);
                if (j == 0)
                    assert((*cube)[i][j][k][LEFT] != NONE);
                else
                    assert((*cube)[i][j][k][LEFT] == NONE);
                if (k == 0)
                    assert((*cube)[i][j][k][FRONT] != NONE);
                else
                    assert((*cube)[i][j][k][FRONT] == NONE);
                if (j == N - 1)
                    assert((*cube)[i][j][k][RIGHT] != NONE);
                else
                    assert((*cube)[i][j][k][RIGHT] == NONE);
                if (k == N - 1)
                    assert((*cube)[i][j][k][BACK] != NONE);
                else
                    assert((*cube)[i][j][k][BACK] == NONE);
                if (i == N - 1)
                    assert((*cube)[i][j][k][BOTTOM] != NONE);
                else
                    assert((*cube)[i][j][k][BOTTOM] == NONE);

            }
        }
    }
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

bool checkSolved(cube_t* cube) {
    int front_color = NONE;
    int back_color = NONE;
    int left_color = NONE;
    int right_color = NONE;
    int top_color = NONE;
    int bottom_color = NONE;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (i == 0) {
                    if (top_color == NONE)
                        top_color = (*cube)[i][j][k][TOP];
                    else if (top_color != (*cube)[i][j][k][TOP])
                        return false;
                }
                if (j == 0) {
                    if (left_color == NONE)
                        left_color = (*cube)[i][j][k][LEFT];
                    else if (left_color != (*cube)[i][j][k][LEFT])
                        return false;
                }
                if (k == 0){
                    if (front_color == NONE)
                        front_color = (*cube)[i][j][k][FRONT];
                    else if (front_color != (*cube)[i][j][k][FRONT])
                        return false;
                }
                if (j == N - 1){
                    if (right_color == NONE)
                        right_color = (*cube)[i][j][k][RIGHT];
                    else if (right_color != (*cube)[i][j][k][RIGHT])
                        return false;
                }
                if (k == N - 1){
                    if (back_color == NONE)
                        back_color = (*cube)[i][j][k][BACK];
                    else if (back_color != (*cube)[i][j][k][BACK])
                        return false;
                }
                if (i == N - 1){
                    if (bottom_color == NONE)
                        bottom_color = (*cube)[i][j][k][BOTTOM];
                    else if (bottom_color != (*cube)[i][j][k][BOTTOM])
                        return false;
                }
            }
        }
    }
    return true;
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