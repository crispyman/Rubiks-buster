//
// Created by andrewiii on 6/15/22.
//

#ifndef RUBIKS_BUSTER_RUBIKS_SEQENTIAL_H
#define RUBIKS_BUSTER_RUBIKS_SEQENTIAL_H

solution_t* seqentialLauncher(cube_t* cube);
rotate_action_t* seqentialSolver(cube_t *cube, rotate_action_t action, int step);



#endif //RUBIKS_BUSTER_RUBIKS_SEQENTIAL_H
