//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include "HELPERS.h"
#include "RUBIKS_SEQENTIAL.h"

int best_length = -1;

rotate_action_t best_solution[MAX_SOLUTION_LENGTH] = {0};

rotate_action_t current_solution[MAX_SOLUTION_LENGTH] = {0};

solution_t* seqentialLauncher(cube_t* cube) {
    // Initialize a solution.
    solution_t* solution = calloc(1, sizeof(solution_t));

    // Consider each possible action.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 2; k++) {
                // Define the new action.
                rotate_action_t new_action = {
                    .index = i,
                    .a = j,
                    .cc = k
                };

                // Create a copy of the cube.
                cube_t* cube_copy = malloc(sizeof(cube_t));
                memcpy(cube_copy, cube, sizeof(cube_t));

                // Call sequential solver with the newly created action and the
                // copy fo the cube.
                seqentialSolver(cube_copy, new_action, 0);

                // Free the cube copy.
                free(cube_copy);
            }
        }
    }

    // Return the best solution.
    solution->steps = best_solution;
    solution->length = best_length;
    return solution;
}

/*
 * Recursive function that tries to find all possible rubix cube solutions up to a certain
 * depth, Note: will not recurse past a valid solution since any solution past that point is
 * definitely longer than the best solution
 */
void seqentialSolver(cube_t* cube, rotate_action_t action, int step) {
    // Apply the rotation to the cube
    rotate(cube, action);

    // if debugging make sure each step results in valid output, NOTE: Probably very slow
    if (DEBUG) {
        printf("step: %d\n", step);
        printf("index: %d, side: %d, cc: %d\n", action.index, action.a, action.cc);
        verifyValid(cube);
    }

    // Move the new action to the current_solution.
    memcpy(&current_solution[step], &action, sizeof(rotate_action_t));

    // Increment the step.
    step++;

    // If we find a solution.
    if (checkSolved(cube)){
        if (DEBUG) printf("Found Solution in %d steps\n", step);

        // Check if it's shorter than current best.
        if (best_length == -1 || step < best_length) {
            memcpy(best_solution, current_solution, sizeof(rotate_action_t) * step);
            best_length = step;
        }
    }
    // If we just need to keep recursing
    else if ((best_length == -1 || step < best_length) && step < MAX_SOLUTION_LENGTH) {
        // permute over all the states that lead away from here
        for (int i = 0; i < N; i++){
            for (int j = 0; j < 3; j++){
                for (int k = 0; k < 2; k++) {
                    rotate_action_t new_action = {
                        .index = i,
                        .a = j,
                        .cc = k
                    };

                    // Create copy of current cube state to send off
                    cube_t cube_copy;
                    memcpy(&cube_copy, cube, sizeof(cube_t));

                    seqentialSolver(&cube_copy, new_action, step);
                }
            }
        }
    }
}

