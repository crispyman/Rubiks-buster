#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<mpi/mpi.h>
//#include <crt/math_functions.h>
#include "HELPERS.h"
#include "RUBIKS_PARALLEL.h"

rotate_action_t* parallelSolver(cube_t *cube, rotate_action_t action, int step, int *best_length);

MPI_Datatype directType;

MPI_Datatype solutionType;



solution_t* parallelLauncher(cube_t* cube, solution_t * solution) {
    int myId;
    int numP;

//    int blockLengths[2] = {1, 6 * N * N};
//    MPI_Aint lb, extent;
//    MPI_Type_get_extent(MPI_INT, &lb, &extent);
//    MPI_Aint disp[2] = {0, 2 * extent};
//    MPI_Datatype types[2] = {MPI_INT, MPI_CHAR};
//
//    MPI_Type_create_struct(2, blockLengths, disp, types, &directType);
//    MPI_Type_commit(& directType);


    int blockLengthsSolution[2] = {1, 6 * N * N};
    MPI_Aint lbSolution, extentSolution;
    MPI_Type_get_extent(MPI_INT, &lbSolution, &extentSolution);
    MPI_Aint dispSolution[2] = {0, 2 * extentSolution};
    MPI_Datatype typesSolution[2] = {MPI_INT, MPI_CHAR};

    MPI_Type_create_struct(2, blockLengthsSolution, dispSolution, typesSolution, &solutionType);
    MPI_Type_commit(& solutionType);


    solution_t * best_solution = malloc(sizeof(solution_t) * MAX_SOLUTION_LENGTH);
    int best_length = MAX_SOLUTION_LENGTH;

    MPI_Comm_size(MPI_COMM_WORLD, &numP);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    //solution_t *solution;

    if (!myId) {
        printf("Number of processes: %d\n", numP);
        int num_actions = N * 3 * 2;
        rotate_action_t actions[num_actions];
        int next = -1;

        solution = calloc(1, sizeof(solution_t));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 2; k++) {
                    rotate_action_t new_action;
                    new_action.index = i;
                    new_action.a = j;
                    new_action.cc = k;

                    memcpy(&actions[++next], &new_action, sizeof(rotate_action_t));


                }
            }
        }

        for (next = 0; next < min(numP, num_actions); next++){
            MPI_Send(&actions[next], 3, MPI_INT, next+1, 0, MPI_COMM_WORLD);
        }

        for (; next < num_actions; next++){
            solution_t *temp_solution = malloc(sizeof(solution_t) * MAX_SOLUTION_LENGTH);

            MPI_Status status;

            MPI_Recv(temp_solution, 1, solutionType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&actions[next], 3, MPI_INT, next, 0, MPI_COMM_WORLD);


            if (temp_solution->length < best_length){
                memcpy(solution->steps, temp_solution, sizeof(rotate_action_t) * best_length);
                solution->length = best_length;
            }

        }

        for (int i = 0; i < numP; i++) {
            solution_t *temp_solution = malloc(sizeof(solution_t) * MAX_SOLUTION_LENGTH);


            MPI_Recv(temp_solution, 1, solutionType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (temp_solution->length < best_length) {
                memcpy(solution->steps, temp_solution, sizeof(rotate_action_t) * best_length);
                solution->length = best_length;
            }
        }


    }

    else{
        cube_t cube_copy;// = malloc(sizeof(cube_t));
        memcpy(&cube_copy, cube, sizeof(cube_t));

        rotate_action_t next_action;

        rotate_action_t * action_chain;

        solution_t current_solution;


        MPI_Recv(&next_action, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        action_chain = parallelSolver((cube_t *) cube_copy, next_action, 0, &best_length);

        current_solution.length =best_length;
        memcpy(current_solution.steps, action_chain, sizeof(rotate_action_t) * best_length);
        free(action_chain);

        MPI_Send(&current_solution, 1, solutionType, 0, 0, MPI_COMM_WORLD);


    }


    MPI_Finalize();


}


/*
 * Recursive function that tries to find all possible rubix cube solutions up to a certain
 * depth, Note: will not recurse past a valid solution since any solution past that point is
 * definitely longer than the best solution
 */
rotate_action_t* parallelSolver(cube_t *cube, rotate_action_t action, int step, int *best_length) {


    rotate(cube, action);

    // if debugging make sure each step results in valid output, NOTE: Probably very slow
    if (DEBUG) {
        printf("step: %d\n", step);
        printf("index: %d, side: %d, cc: %d\n", action.index, action.a, action.cc);
        verifyValid(cube);
    }
    // if we find a solution
    if (checkSolved(cube)){
        print_cube(cube);
        // check if it's shorter than current best
        if (step < *best_length) {
            rotate_action_t * solutions = calloc(step + 1, sizeof(rotate_action_t));
            memcpy(&solutions[step], &action, sizeof(rotate_action_t));
            return solutions;
        } else
            return NULL;
    }
        // we recurse to far, turn back, there be dragons (unallocated memory)
    else if (step >= *best_length)
        return NULL;
        // If we just need to keep recursing
    else {
        rotate_action_t *best_solution = NULL;

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
                    rotate_action_t *temp_solution = parallelSolver(&cube_copy, new_action, step + 1, best_length);
                    // check if there is a new solution returned, if so we know its shorter and its length is of global
                    // best length
                    if (temp_solution){
                        if (best_solution)
                            free(best_solution);
                        best_solution = temp_solution;
                    }
                    // get rid of the cube copy we made since the function using it returned;
                    //free(cube_copy);
                }
            }
        }
        if (best_solution)
            memcpy(&best_solution[step], &action, sizeof(rotate_action_t));
        return best_solution;
    }







}

#pragma clang diagnostic pop