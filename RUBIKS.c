//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<mpi/mpi.h>
#include "RUBIKS.h"
#include "HELPERS.h"
#include "RUBIKS_SEQENTIAL.h"
#include "RUBIKS_PARALLEL.h"

// Proototypes.
void initialize(cube_t* cube);
void scramble(cube_t* cube);


int main(int argc, char * argv[]) {
    MPI_Init(NULL, NULL);
    int myId;
    int numP;
    cube_t *my_cube;
    solution_t parallel_solution;


    //rotate_action_t * best_solution = malloc(sizeof(rotate_action_t) *
    MPI_Comm_size(MPI_COMM_WORLD, &numP);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);
    if (!myId) {
        srand(time(0));
        my_cube = malloc(sizeof(cube_t));
        initialize(my_cube);
        verifyValid(my_cube);
        printf("initialize produces valid output\n");

        print_cube(my_cube);
        printf("\n\n");

        scramble(my_cube);
        verifyValid(my_cube);
        printf("scramble produces valid output\n");
        //cube_t * seq_cube = malloc(sizeof(cube_t));
        //memcpy(seq_cube, my_cube, sizeof(cube_t));
//    solution_t * seq_solution = seqentialLauncher(my_cube);
//    if (seq_solution->length){
//        printf("Solved in: %d steps\n", seq_solution->length);
//    }
//    else
//        printf("No Solution in %d steps\n", MAX_SOLUTION_LENGTH);


    }
    parallelLauncher(my_cube, &parallel_solution);

    if (!myId) {
        if (parallel_solution.length) {
            printf("Solved in: %d steps\n", parallel_solution.length);
        } else
            printf("No Solution in %d steps\n", MAX_SOLUTION_LENGTH);
        print_cube(my_cube);

        free(my_cube);

    }


    MPI_Finalize();







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
            .index = rand() % N,
            .a = rand() % 3,
            .cc = rand() % 2,
        };
        rotate(cube, action);
    }
}

