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

    verifyValid(my_cube);

    print_cube(my_cube);


    free(my_cube);
}

// void initialize(cube_t* cube){
//     for (int i = 0; i < SIDES; i++){
//         for (int j = 0; j < N; j++) {
//             for (int k = 0; k < N; k ++){
//                     if (i == TOP)
//                         (*cube)[i][j][k] = WHITE;
//                     if (i == LEFT)
//                         (*cube)[i][j][k] = GREEN;
//                     if (i == FRONT)
//                         (*cube)[i][j][k] = RED;
//                     if (i == RIGHT)
//                         (*cube)[i][j][k] = BLUE;
//                     if (i == BACK)
//                         (*cube)[i][j][k] = ORANGE;
//                     if (i == BOTTOM)
//                         (*cube)[i][j][k] = YELLOW;
//             }
//         }
//     }
// }
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

