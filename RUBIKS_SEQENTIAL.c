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

bool seqentialSolver(cube_t* cube, rotate_action_t *action, int step, int max_steps) {
    int temp_rand = rand();
    rotate_action_t new_action;
    //do {
        new_action.index = rand() % N;
        new_action.a = rand() % 3;
        new_action.cc = rand() % 2;


    //}while(memcmp(&new_action, &action[step-1], sizeof(rotate_action_t)));

    rotate(cube, new_action);

    if (DEBUG)
        verifyValid(cube);

    memcpy(&action[step], &new_action, sizeof(rotate_action_t));
    if (checkSolved(cube))
        return true;
    if (step == max_steps -1)
        return false;
    else
        seqentialSolver(cube, action, step++, max_steps);







}

