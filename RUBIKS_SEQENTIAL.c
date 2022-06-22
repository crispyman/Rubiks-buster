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

int best_length = MAX_SOLUTION_LENGTH;

rotate_action_t best_solution[MAX_SOLUTION_LENGTH + 1] = {0};

rotate_action_t current_solution[MAX_SOLUTION_LENGTH + 1] = {0};

solution_t* seqentialLauncher(cube_t* cube) {
    solution_t *solution = calloc(1, sizeof(solution_t));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 2; k++) {
                rotate_action_t new_action;
                new_action.index = i;
                new_action.a = j;
                new_action.cc = k;

                cube_t *cube_copy = malloc(sizeof(cube_t));
                memcpy(cube_copy, cube, sizeof(cube_t));
                seqentialSolver(cube_copy, new_action, 0);

                free(cube_copy);
            }
        }
    }
    solution->steps = best_solution;
    solution->length = best_length;
    return solution;

}
/*
 * Recursive function that tries to find all possible rubix cube solutions up to a certain
 * depth, Note: will not recurse past a valid solution since any solution past that point is
 * definitely longer than the best solution
 */
void seqentialSolver(cube_t *cube, rotate_action_t action, int step) {
    // Apply the rotation to the cube
    rotate(cube, action);

    // if debugging make sure each step results in valid output, NOTE: Probably very slow
    if (DEBUG) {
        printf("step: %d\n", step);
        printf("index: %d, side: %d, cc: %d\n", action.index, action.a, action.cc);
        verifyValid(cube);
    }

    //stack_push(current_solution, action);
    memcpy(&current_solution[step], &action, sizeof(rotate_action_t));
    // if we find a solution
    if (checkSolved(cube)){
        if (DEBUG) printf("Found Solution in %d steps\n", step + 1);
        // check if it's shorter than current best
        if (step+1 < best_length) {
            memcpy(best_solution, current_solution, sizeof(rotate_action_t) * (step + 1));
            best_length = step+1;
        }
    }
    // If we just need to keep recursing
    else if (step < best_length-1) {

        // permute over all the states that lead away from here
        // TODO: don't go back to our last state and preferably any previous state
        for (int i = 0; i < N; i++){
            for (int j = 0; j < 3; j++){
                for (int k = 0; k < 2; k++) {
                    rotate_action_t new_action;
                    new_action.index = i;
                    new_action.a = j;
                    new_action.cc = k;

                    // create copy of current cube state to send off
                    cube_t cube_copy;// = malloc(sizeof(cube_t));
                    memcpy(&cube_copy, cube, sizeof(cube_t));
                    seqentialSolver(&cube_copy, new_action, step + 1);
                    // check if there is a new solution returned, if so we know its shorter and its length is of global
                    // best length
                    // get rid of the cube copy we made since the function using it returned;
                    //free(cube_copy);
                }
            }
        }
    }
}

