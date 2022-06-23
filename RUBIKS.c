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
#include "RUBIKS_PARALLEL.h"

// Prototypes.
void initialize(cube_t* cube);
void scramble(cube_t* cube);

char axis_char[3] = {'X', 'Y', 'Z'};

int main(int argc, char * argv[]) {
    // Initialize MPI.
    MPI_Init(NULL, NULL);

    // Variables used in main.
    int myId;
    int numP;
    cube_t *my_cube;
    solution_t para_solution;
    double seq_time = 0;
    double para_time = 0;

    // Define the timer.
    clock_t t = 0;

    // Get the number of processes and the current process index.
    MPI_Comm_size(MPI_COMM_WORLD, &numP);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    if (!myId) {
        // Set the random seed.
        srand(time(0));

        // Initialize the cube.
        printf("Initializing a %d × %d cube...\n", N, N);
        my_cube = malloc(sizeof(cube_t));
        initialize(my_cube);
        verifyValid(my_cube);
        printf("Initialize produces valid output:\n");
        print_cube(my_cube);
        printf("\n");

        // Scramble the cube.
        printf("Performing %d random rotations on the cube...\n", N_SCRAMBLES);
        scramble(my_cube);
        verifyValid(my_cube);
        printf("Scramble produces valid output:\n");
        print_cube(my_cube);
        printf("\n");

        // Make a copy of the cube for the sequential version to work on.
        cube_t* seq_cube = malloc(sizeof(cube_t));
        memcpy(seq_cube, my_cube, sizeof(cube_t));

        // Call and time the sequential solution.
        t = clock();
        solution_t * seq_solution = seqentialLauncher(seq_cube);
        t = clock() - t;

        // Convert the sequential time to seconds.
        seq_time = ((double)t) / CLOCKS_PER_SEC;

        // Print whether it was successfully solved.
        if (seq_solution->length == -1) {
            printf("Seqential solver failed to solve the cube in %d steps\n", MAX_SOLUTION_LENGTH);
        } else {
            printf("Sequential solver solved the cube in %d steps:\n", seq_solution->length);
            printf("STEP\tAXIS\tINDEX\tDIRECTION\n");
            for (int i = 0; i < seq_solution->length; i++) {
                printf("%d\t", i);
                printf("%c\t", axis_char[seq_solution->steps[i].a]);
                printf("%d\t", seq_solution->steps[i].index);
                printf("%s\n", seq_solution->steps[i].cc ? "cc" : "c");
            }
        }

        // Free the original cube.
        //free(my_cube);

        // Free the sequential copy of the cube.
        free(seq_cube);
    }

    // Ensure that sequential is completed before moving on to parallel.
    MPI_Barrier(MPI_COMM_WORLD);

    // Run and time the parallel solution.
    if (!myId) t = clock();
    parallelLauncher(my_cube, &para_solution);
    MPI_Barrier(MPI_COMM_WORLD);
    if(!myId) t = clock() - t;

    if (!myId) {
        // Convert the parallel time to seconds.
        para_time = ((double)t) / CLOCKS_PER_SEC;

        // Print whether it was successfully solved.
        if (para_solution.length <= MAX_SOLUTION_LENGTH) {
            printf("Parallel solver solved the cube in %d steps\n", para_solution.length);
            printf("STEP\tAXIS\tINDEX\tDIRECTION\n");
            for (int i = 0; i < para_solution.length; i++) {
                printf("%d\t", i);
                printf("%c\t", axis_char[para_solution.steps[i].a]);
                printf("%d\t", para_solution.steps[i].index);
                printf("%s\n", para_solution.steps[i].cc ? "cc" : "c");
            }
        } else {
            printf("Parallel solver failed to solve the cube in %d steps\n", MAX_SOLUTION_LENGTH);

        }

        // Print speeds and speedup.
        printf("Sequential speed:\t%fs\n", seq_time);
        printf("Parallel speed:\t\t%fs\n", para_time);
        printf("Speedup:\t\t%f\n", seq_time / para_time);
    }

    // Finalize MPI.
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

