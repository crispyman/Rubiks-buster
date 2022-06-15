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

void initialize(cube_t* cube);

int main(int argc, char * argv[]) {
    cube_t *my_cube = malloc(N * N * N * 6 * sizeof(int));
    initialize(my_cube);





    free(my_cube);

}
void initialize(cube_t* cube){
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k ++){
                for (int l = 0; l < N; l++){
                    if (i == 0)
                        (*cube)[i][j][k][TOP] = WHITE;
                    else
                        (*cube)[i][j][k][TOP] = NONE;
                    if (j == 0)
                        (*cube)[i][j][k][LEFT] = GREEN;
                    else
                        (*cube)[i][j][k][LEFT] = NONE;
                    if ( l == 0)
                        (*cube)[i][j][l][FRONT] = RED;
                    else
                        (*cube)[i][j][k][FRONT] = NONE;
                    if ( j == N-1)
                        (*cube)[i][j][l][RIGHT] = BLUE;
                    else
                        (*cube)[i][j][l][RIGHT] = NONE;
                    if ( l == N-1)
                        (*cube)[i][j][l][BACK] = ORANGE;
                    else
                        (*cube)[i][j][l][BACK] = NONE;
                    if ( i == N-1)
                        (*cube)[i][j][l][BOTTOM] = YELLOW;
                    else
                        (*cube)[i][j][l][BOTTOM] = NONE;
                }
            }
        }
    }
}