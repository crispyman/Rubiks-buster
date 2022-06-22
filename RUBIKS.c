//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <time.h>
#include "mpi.h"
#include "RUBIKS.h"
#include "HELPERS.h"
#include "RUBIKS_SEQENTIAL.h"

// Prototypes.
void initialize(cube_t* cube);
void scramble(cube_t* cube);


int main(int argc, char * argv[]) {
    // Set the random seed.
    srand(time(0));

    // Define the timer.
    clock_t t;

    // Initialize the cube.
    cube_t *my_cube = malloc(sizeof(cube_t));
    initialize(my_cube);
    verifyValid(my_cube);
    printf("Initialize produces valid output:\n");
    print_cube(my_cube);
    printf("\n");

    // Scramble the cube.
    scramble(my_cube);
    verifyValid(my_cube);
    printf("Scramble produces valid output:\n");
    print_cube(my_cube);
    printf("\n");

    // Make two copies of the cube, one for each solution to work on.
    cube_t* seq_cube = malloc(sizeof(cube_t));
    memcpy(seq_cube, my_cube, sizeof(cube_t));
    cube_t* para_cube = my_cube;

    // Run the sequential solver and time its execution.
    t = clock();
    solution_t* seq_solution = seqentialLauncher(seq_cube);
    t = clock() - t;

    if (DEBUG) {
        for (int i = 0; i < seq_solution->length; i++) {
            printf("%d\t%d\t%d\n", seq_solution->steps[i].a, seq_solution->steps[i].index, seq_solution->steps[i].cc);
        }
    }
    
    // Free the sequential copy of the cube.
    free(seq_cube);

    // Convert the sequential time to seconds.
    double seq_time = ((double)t) / CLOCKS_PER_SEC;

    // Run the parallel solver and time its execution.
    t = clock();
    // PLACE CALL TO PARALLEL HERE
    t = clock() - t;

    // Free the parallel copy of the cube.
    free(para_cube);

    // Convert the parallel time to seconds.
    double para_time = ((double)t) / CLOCKS_PER_SEC;

    printf("Sequential speed:\t%fs\n", seq_time);
    printf("Parallel speed:\t\t%fs\n", para_time);
    printf("Speedup:\t\t%f\n", seq_time / para_time);
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

