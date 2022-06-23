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


int main(int argc, char * argv[]) {
    MPI_Init(NULL, NULL);
    int myId;
    int numP;
    cube_t *my_cube;
    solution_t parallel_solution;
    clock_t t;
    double seq_time;



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
    // Set the random seed.


    // Define the timer.
    clock_t t;

    // Initialize the cube.
    cube_t *my_cube = malloc(sizeof(cube_t));
    initialize(my_cube);
    verifyValid(my_cube);
    printf("Initialize produces valid output:\n");
    print_cube(my_cube);
    printf("\n");

        scramble(my_cube);
        verifyValid(my_cube);
        printf("scramble produces valid output\n");
        cube_t * seq_cube = malloc(sizeof(cube_t));
        memcpy(seq_cube, my_cube, sizeof(cube_t));
        t = clock();

        solution_t * seq_solution = seqentialLauncher(my_cube);

        t = clock() - t;

        seq_time = ((double)t) / CLOCKS_PER_SEC;


        if (seq_solution->length == -1) {
            printf("Seqential solver failed to solve the cube in %d steps\n", MAX_SOLUTION_LENGTH);
        } else {
            // printf("Parallel solver solved the cube in %d steps\n", para_solution->length);
            printf("Sequential solver solved the cube in %d steps\n", seq_solution->length);
        }

        free(my_cube);

        if (DEBUG) {
            for (int i = 0; i < seq_solution->length; i++) {
                printf("%d\t%d\t%d\n", seq_solution->steps[i].a, seq_solution->steps[i].index, seq_solution->steps[i].cc);
            }
        }

        free(seq_cube);



    }


    // Free the sequential copy of the cube.
    MPI_Barrier(MPI_COMM_WORLD);

    if (!myId)
        t = clock();

    parallelLauncher(my_cube, &parallel_solution);
    MPI_Barrier(MPI_COMM_WORLD);

    if(!myId)
        t = clock() - t;

    // Convert the sequential time to seconds.

    if (!myId) {
        if (parallel_solution.length <= MAX_SOLUTION_LENGTH) {
            printf("Solved in: %d steps\n", parallel_solution.length);
        } else
            printf("No Solution in %d steps\n", MAX_SOLUTION_LENGTH);
        print_cube(my_cube);
        // Run the parallel solver and time its execution.
        // PLACE CALL TO PARALLEL HERE

        // Print whether it was successfully solved.
        double para_time = ((double)t) / CLOCKS_PER_SEC;

        printf("Sequential speed:\t%fs\n", seq_time);
        printf("Parallel speed:\t\t%fs\n", para_time);
        printf("Speedup:\t\t%f\n", seq_time / para_time);


    }

    MPI_Finalize();







    // Free the parallel copy of the cube.

    // Convert the parallel time to seconds.


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

