#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<mpi/mpi.h>
#include "HELPERS.h"
#include "RUBIKS_PARALLEL.h"

rotate_action_t* parallelSolver(cube_t *cube, rotate_action_t action, int step, int *best_length);


MPI_Datatype solutionType;



void parallelLauncher(const cube_t* const cube, solution_p_t * const solution) {
    int myId;
    int numP;
    solution->length = MAX_SOLUTION_LENGTH+1;


    // Struct for sending the solution back to proccess 0
    int blockLengthsSolution[2] = {1, 6 * N * N};
    MPI_Aint lbSolution, extentSolution;
    MPI_Type_get_extent(MPI_INT, &lbSolution, &extentSolution);
    MPI_Aint dispSolution[2] = {0, 2 * extentSolution};
    MPI_Datatype typesSolution[2] = {MPI_INT, MPI_CHAR};

    MPI_Type_create_struct(2, blockLengthsSolution, dispSolution, typesSolution, &solutionType);
    MPI_Type_commit(&solutionType);


    //solution_t * best_solution = malloc(sizeof(solution_t) * MAX_SOLUTION_LENGTH);
    int best_length = MAX_SOLUTION_LENGTH+1;

    MPI_Comm_size(MPI_COMM_WORLD, &numP);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);

    // proc 0 setup
    if (!myId) {
        //printf("Number of processes: %d\n", numP);
        int num_actions = N * 3 * 2;
        rotate_action_t actions[num_actions];
        int next = -1;
        // Generates all the launch actions ahead of time
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
        // sends Cube to processes
        for (int i = 1; i < numP; i++){
            //printf("data Sent %d\n", next+1);
            MPI_Send(cube, SIDES * N * N, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }

        // sends first batch of starting positions out
        for (next = 0; next < min(numP-1, num_actions); next++){
            //printf("data Sent %d\n", next+1);
            MPI_Send(&actions[next], 3, MPI_INT, next+1, 0, MPI_COMM_WORLD);
        }

        // reads data and sends remaining jobs out
        for (; next < num_actions; next++){
            solution_p_t temp_solution;

            MPI_Status status;

            MPI_Recv(&temp_solution, 1, solutionType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            //printf("send addr %d", status.MPI_SOURCE);
            MPI_Send(&actions[next], 3, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            //printf("sent addr %d", status.MPI_SOURCE);


            if (temp_solution.length < best_length && temp_solution.length > 0){
                memcpy(&solution->steps, temp_solution.steps, sizeof(rotate_action_t) *  temp_solution.length);
                best_length = temp_solution.length;
                solution->length = best_length;
            }

        }

        // recieves remaining data and tells processes to terminate
        for (int i = 1; i < numP; i++) {
            solution_p_t temp_solution;
            temp_solution.length = MAX_SOLUTION_LENGTH+1;

            MPI_Status status;


            MPI_Recv(&temp_solution, 1, solutionType, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            //printf("GUARD\n %d \nGUARD\n", temp_solution.length);


            if (temp_solution.length < best_length && temp_solution.length > 0){
                memcpy(&solution->steps, temp_solution.steps, sizeof(rotate_action_t) *  temp_solution.length);
                best_length = temp_solution.length;
                solution->length = best_length;
            }

            MPI_Send(NULL, 0, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

        }


    }
    // child process work
    else{
        int local_best_length = best_length;
        cube_t cube_local;
        // get start cube
        MPI_Recv(&cube_local, SIDES * N * N, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cube_t cube_copy;

        memcpy(&cube_copy, &cube_local, sizeof(cube_t));

        rotate_action_t next_action;

        rotate_action_t * action_chain;

        solution_p_t current_solution;
        current_solution.length = MAX_SOLUTION_LENGTH+1;

        MPI_Status status;
        int data_length;

        // get first instruction
        MPI_Recv(&next_action, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        MPI_Get_count(&status, MPI_INT, &data_length);
        while(data_length > 0){
            action_chain = parallelSolver((cube_t *) cube_copy, next_action, 0, &local_best_length);

            //printf("GUARD\n %d \nGUARD\n", local_best_length);

            // if new solution better than local best replace local best
            if (local_best_length < best_length && action_chain != NULL){
                current_solution.length = local_best_length;
                best_length = local_best_length;
                memcpy(&current_solution.steps, &action_chain, sizeof(rotate_action_t) * (local_best_length));
            }
            free(action_chain);


            // send solution
            MPI_Send(&current_solution, 1, solutionType, 0, 0, MPI_COMM_WORLD);
            //printf("data send b %d\n", myId);

            //recv next instruction or terminate command
            MPI_Recv(&next_action, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            // if size 0 message terminate
            MPI_Get_count(&status, MPI_INT, &data_length);

        }
    }



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
    if (step+1 >= *best_length)
        return NULL;

    else if (checkSolved(cube)) {

        // check if it's shorter than current best
        *best_length = step + 1;
        rotate_action_t *solutions = calloc(step + 1, sizeof(rotate_action_t));
        memcpy(&solutions[step], &action, sizeof(rotate_action_t));
        return solutions;

    }
        // we recurse to far, turn back, there be dragons (unallocated memory)

        // If we just need to keep recursing
    else {
        rotate_action_t *best_solution = NULL;

        // permute over all the states that lead away from here
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
                }
            }
        }
        if (best_solution)
            memcpy(&best_solution[step], &action, sizeof(rotate_action_t));
        return best_solution;
    }

}

