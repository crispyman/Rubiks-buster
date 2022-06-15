//
// Created by andrewiii on 6/15/22.
//

#include <stdio.h>
#include <jerror.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"
#include "RUBIKS.h"
#include "HELPERS.h"

int main(int argc, char * argv[]) {
    cube_t *my_cube = malloc(N * N * N * 6 * sizeof(int));

}
void initialize(cube_t* cube){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k ++){
                for (int l = 0; l < N; l++){
                    (*cube)[i][j][k][TOP] = WHITE;
                    (*cube)[i][j][k][LEFT] = GREEN;
                    (*cube)[i][j][l][FRONT] = RED;
                    (*cube)[i][j][l][RIGHT] = BLUE;
                    (*cube)[i][j][l][BACK] = ORANGE;
                    (*cube)[i][j][l][BOTTOM] = YELLOW;
                }
            }
        }
    }
}