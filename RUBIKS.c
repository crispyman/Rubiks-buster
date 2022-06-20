//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"
#include "RUBIKS.h"
#include "HELPERS.h"

// Proototypes.
void initialize(cube_t* cube);
void scramble(cube_t* cube);


int main(int argc, char * argv[]) {
    srand(time(0));
    cube_t *my_cube = malloc(N * N * SIDES * sizeof(int));
    initialize(my_cube);
    verifyValid(my_cube);
    printf("initialize produces valid output\n");

    print_cube(my_cube);

    scramble(my_cube);

    print_cube(my_cube);

    free(my_cube);
}

// Initialize each of the 6 sides of the cube to the appropriate color.
void initialize(cube_t* cube){
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k ++){
            (*cube)[TOP][j][k] = WHITE;
            (*cube)[LEFT][j][k] = GREEN;
            (*cube)[FRONT][j][k] = RED;
            (*cube)[RIGHT][j][k] = BLUE;
            (*cube)[BACK][j][k] = ORANGE;
            (*cube)[BOTTOM][j][k] = YELLOW;
        }
    }
}

// Perform N_SCRAMBLES random rotations on a provided cube.
void scramble(cube_t* cube) {
    for (unsigned int i = 0; i < N_SCRAMBLES; i++) {
        rotate_action_t action = {
            .index = rand() % 3,
            .a = rand() % 3,
            .cc = rand() % 2,
        };
        rotate(cube, action);
    }
}

