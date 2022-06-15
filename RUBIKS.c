//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "RUBIKS.h"

int main(int argc, char * argv[]){

  cube_t* my_cube;

}

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

void rotateFace(color_t* face, int cc) {
  color_t temp_color;
  if (cc) {
    temp_color = face[0];
    face[0] = face[2];
    face[2] = face[7];
    face[7] = face[5];
    face[5] = temp_color;

    temp_color = face[1];
    face[1] = face[4];
    face[4] = face[6];
    face[3] = face[1];
    face[6] = temp_color;
    
  } else {
    temp_color = face[0];
    face[0] = face[5];
    face[5] = face[7];
    face[7] = face[2];
    face[2] = temp_color;

    temp_color = face[1];
    face[1] = face[3];
    face[3] = face[6];
    face[6] = face[4];
    face[4] = temp_color;
  }
}