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

void initialize(cube_t* cube);
void verifyValid(cube_t* cube);
void scramble(cube_t* cube);


int main(int argc, char * argv[]) {
    srand(time(0));
    cube_t *my_cube = malloc(N * N * N * 6 * sizeof(int));
    initialize(my_cube);

    scramble(my_cube);

    verifyValid(my_cube);


    free(my_cube);
}

void initialize(cube_t* cube){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k ++){
                    if (i == 0)
                        (*cube)[i][j][k][TOP] = WHITE;
                    else
                        (*cube)[i][j][k][TOP] = NONE;
                    if (j == 0)
                        (*cube)[i][j][k][LEFT] = GREEN;
                    else
                        (*cube)[i][j][k][LEFT] = NONE;
                    if ( k == 0)
                        (*cube)[i][j][k][FRONT] = RED;
                    else
                        (*cube)[i][j][k][FRONT] = NONE;
                    if ( j == N-1)
                        (*cube)[i][j][k][RIGHT] = BLUE;
                    else
                        (*cube)[i][j][k][RIGHT] = NONE;
                    if ( k == N-1)
                        (*cube)[i][j][k][BACK] = ORANGE;
                    else
                        (*cube)[i][j][k][BACK] = NONE;
                    if ( i == N-1)
                        (*cube)[i][j][k][BOTTOM] = YELLOW;
                    else
                        (*cube)[i][j][k][BOTTOM] = NONE;
            }
        }
    }
}

void scramble(cube_t* cube) {

    for (unsigned int i = 0; i < 2; i++) {
        int temp_rand = rand();
        rotate_action_t action = {
            .x = rand() % N,
            .y = rand() % N,
            .z = rand() % N,
            .a = rand() % 6,
            .cc = rand() % 2,
        };

        rotate(cube, action);
    }
}

