#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include "HELPERS.h"


/* unixError
 * Called when an error occurs to print an error message and exit.
 * Borrowed from Dr. Norris's OS Labs, Thank You!
*/
void unixError(char * msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}


// Picks which rotate function to call.
void rotate(cube_t *cube, rotate_action_t act) {
    if (act.a == XY) {
        rotateX(cube, act.index, act.cc);
    } else if (act.a == YZ) {
        rotateY(cube, act.index, act.cc);
    } else if (act.a == XZ) {
        rotateZ(cube, act.index, act.cc);
    }
}

// NOTES ON ALL: For reference for clockwise or counterclockwise I use the
// perspective of front, right, or top, like in the images.
// Look at cycles in HELPERS.h to see clockwise cycles
// I used memcpy for no reason except speed.

//     +-----------+
//    /           /|
//   /           / |
//  +-----------+  |
//  |           |  |
//  |           |  +
//  |           | /
//  | --------▶ |/
//  +-----------+
void rotateX(cube_t *cube, int row, int cc) {
    int temp[N];

    if (!cc) {
        // Copies start cube into temp space starting with first side (FRONT)
        memcpy(temp, (*cube)[cycleX[0]][row], N * sizeof(int));
        // Copies each row back to the previous side
        for (int i = 0; i < 3; i++) {
            memcpy((*cube)[cycleX[i]][row], (*cube)[cycleX[i + 1]][row], N * sizeof(int));
        }
        // Copies temp back into last row (LEFT)
        memcpy((*cube)[cycleX[3]][row], temp, N * sizeof(int));
    } else {
        // Copies start cube into temp space starting with last side (LEFT)
        memcpy(temp, (*cube)[cycleX[3]][row], N * sizeof(int));
        // Copies each row to the next side
        for (int i = 3; i > 0; i--) {
            memcpy((*cube)[cycleX[i]][row], (*cube)[cycleX[i-1]][row], N * sizeof(int));
        }
        // Copies temp back into First row (Front)
        memcpy((*cube)[cycleX[0]][row], temp, N * sizeof(int));
    }

    // Call rotate face if it's on an edge
    if (row == 0) {
        rotate_face(cube, TOP, cc);
    } else if (row == N - 1) {
        rotate_face(cube, BOTTOM, cc);

    }
}

//     +-----------+
//    /           /|
//   /           / |
//  +-----------+  |
//  |    ▲      |  |
//  |    |      |  +
//  |    |      | /
//  |    |      |/
//  +-----------+
void rotateY(cube_t *cube, int col, int cc) {
    int temp[N];
    if (!cc) {
        // Copies start cube into temp space starting with first side (FRONT)
        for (int i = 0; i < N; i++) {
            temp[i] = (*cube)[cycleY[0]][i][col];
        }
        // Copies each column to the next side
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < N; j++) {
                (*cube)[cycleY[i]][j][col] = (*cube)[cycleY[i + 1]][j][col];
            }
        }
        // Copies temp back into last row
        for (int i = 0; i < N; i++) {
            (*cube)[cycleY[3]][i][col] = temp[i];

        }
    } else {
        // Copies start cube into temp space starting with first side (BOTTOM)
        for (int i = 0; i < N; i++) {
            temp[i] = (*cube)[cycleY[3]][i][col];
        }
        // Copies each column to the next side
        for (int i = 3; i > 0; i--) {
            for (int j = 0; j < N; j++) {
                (*cube)[cycleY[i + 1]][j][col] = (*cube)[cycleY[i]][j][col];
            }
        }
        // Copies temp back into last row (FRONT)
        for (int i = 0; i < N; i++) {
            (*cube)[cycleY[0]][i][col] = temp[i];

        }
    }
    // Call rotate face if it's on an edge
    if (col == 0) {
        rotate_face(cube, LEFT, cc);

    } else if (col == N - 1) {
        rotate_face(cube, RIGHT, cc);

    }
}

//     +-----------+
//    /           /|
//   / --------▶ / |
//  +-----------+▲ |
//  |           || |
//  |           || +
//  |           |▼/
//  |           |/
//  +-----------+
void rotateZ(cube_t *cube, int col, int cc) {
    int temp[N];
    if (!cc) {
        // Copies start cube into temp space starting with first side (RIGHT)
        for (int i = 0; i < N; i++) {
            temp[i] = (*cube)[cycleZ[0]][i][col];
        }
        // Copies each column to the next side
        //TODO: Isn't it columns on the sides the rows on Top?
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < N; j++) {
                (*cube)[cycleZ[i]][j][col] = (*cube)[cycleZ[i + 1]][j][col];
            }
        }
        // Copies temp back into last row (TOP)
        for (int i = 0; i < N; i++) {
            (*cube)[cycleZ[3]][i][col] = temp[i];

        }
    } else {
        // Copies start cube into temp space starting with first side (TOP)
        for (int i = 0; i < N; i++) {
            temp[i] = (*cube)[cycleZ[3]][i][col];
        }
        // Copies each column to the next side
        //TODO: Isn't it columns on the sides the rows on Top and bottom?
        for (int i = 3; i > 0; i--) {
            for (int j = 0; j < N; j++) {
                (*cube)[cycleZ[i + 1]][j][col] = (*cube)[cycleZ[i]][j][col];
            }
        }
        // Copies temp back into last row (RIGHT)
        for (int i = 0; i < N; i++) {
            (*cube)[cycleZ[0]][i][col] = temp[i];

        }
    }
    // Call rotate face if it's on an edge
    if (col == 0) {
        rotate_face(cube, FRONT, cc);

    } else if (col == N - 1) {
        rotate_face(cube, BACK, cc);

    }
}

// Just ensures that each spot has a valid color in it, does not check edges
void verifyValid(cube_t *cube) {
    for (int i = 0; i < SIDES; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (!((*cube)[i][j][k] > NONE && (*cube)[i][j][k] <= YELLOW)) {
                    //borrowed from an OS lab, Thanks Dr. Norris!
                    fprintf(stderr, "\033[91mERROR: Side: %d x:%d y: %d Value: %d\033[0m\n", i, j, k, (*cube)[i][j][k]);
                    exit(1);
                }
            }
        }
    }
}


/*
 * Helper for the rotate functions, rotates neighboring side if row getting rotated
 * is on an edge.
 */
void rotate_face(cube_t *cube, side_t side, int cc) {
    color_t temp;
    // This should loop over each ring working inwards only works once rn because 3x3
    for (int i = 0; i < N/2; i++) {
        // works over the length of a segment of the cycle and rotates it
        for (int j = i; j < (N - i); j++) {
            if (cc) {
                /* This for loop defines our cycles around the edges
                 * So if i, j are (0,0) we would store it in temp
                 * Then rotate (3, 0) into (0, 0)
                 * Then rotate (3, 3) into (3, 0)
                 * Then rotate (0, 3) into (3, 3)
                 * Then store temp in (0, 3)
                 */
                for (int k = 0; k < 4; k++) {
                    if (k == 0) {
                        temp = (*cube)[side][i][j];
                        (*cube)[side][i][j] = (*cube)[side][j][N-1 - i];
                    }
                    if (k == 1) {
                        (*cube)[side][j][N-1 - i] = (*cube)[side][N-1 - i][N-1 - j];
                    }
                    if (k == 2) {
                        (*cube)[side][N-1 - i][N-1 - j] = (*cube)[side][N-1 - j][i];
                    }
                    if (k == 3) {
                        (*cube)[side][N-1 - j][i] = temp;
                    }
                }
            } else {
                // The same rotation as above but in reverse
                for (int k = 0; k < 4; k++) {
                    if (k == 0) {
                        temp = (*cube)[side][N - j][i];
                        (*cube)[side][N-1 - j][i] = (*cube)[side][N-1 - i][N-1 - j];
                    }
                    if (k == 1) {
                        (*cube)[side][N-1 - i][N-1 - j] = (*cube)[side][j][N-1 - i];
                    }
                    if (k == 2) {
                        (*cube)[side][j][N-1 - i] = (*cube)[side][i][j];
                    }
                    if (k == 3) {
                        (*cube)[side][i][j] = temp;
                    }
                }
            }
        }
    }
}

//TODO: replace with function to check hashed solution
bool checkSolved(cube_t* cube) {
    int front_color = NONE;
    int back_color = NONE;
    int left_color = NONE;
    int right_color = NONE;
    int top_color = NONE;
    int bottom_color = NONE;
    for (int i = 0; i < SIDES; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (i == 0) {
                    if (top_color == NONE)
                        top_color = (*cube)[i][j][k];
                    else if (top_color != (*cube)[i][j][k])
                        return false;
                }
                if (i == 1) {
                    if (left_color == NONE)
                        left_color = (*cube)[i][j][k];
                    else if (left_color != (*cube)[i][j][k])
                        return false;
                }
                if (i == 2){
                    if (front_color == NONE)
                        front_color = (*cube)[i][j][k];
                    else if (front_color != (*cube)[i][j][k])
                        return false;
                }
                if (i == 3){
                    if (right_color == NONE)
                        right_color = (*cube)[i][j][k];
                    else if (right_color != (*cube)[i][j][k])
                        return false;
                }
                if (i == 4){
                    if (back_color == NONE)
                        back_color = (*cube)[i][j][k];
                    else if (back_color != (*cube)[i][j][k])
                        return false;
                }
                if (i == 5){
                    if (bottom_color == NONE)
                        bottom_color = (*cube)[i][j][k];
                    else if (bottom_color != (*cube)[i][j][k])
                        return false;
                }
            }
        }
    }
    return true;
}
// Given a populated plane, rotate it 90% either clockwise or counter clockwise.


void print_cube(cube_t* cube) {
    // Print top.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) printf(" ");
        for (int j = 0; j < N; j++) print_color((*cube)[TOP][i][j]);
        printf("\n");
    }

    // Print middle.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) print_color((*cube)[LEFT][i][j]);

        for (int j = 0; j < N; j++) print_color((*cube)[FRONT][i][j]);

        for (int j = 0; j < N; j++) print_color((*cube)[RIGHT][i][j]);

        for (int j = 0; j < N; j++) print_color((*cube)[BACK][i][j]);

        printf("\n");
    }

    // Print bottom.
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) printf(" ");
            for (int j = 0; j < N; j++) print_color((*cube)[BOTTOM][i][j]);
        printf("\n");
    }
}

void print_color(color_t color) {
    switch (color) {
        case WHITE: {
            printf("\033[1;37m"); // white
            break;
        }
        case GREEN: {
            printf("\033[1;32m"); // green
            break;
        }
        case RED: {
            printf("\033[1;31m"); // red
            break;
        }
        case BLUE: {
            printf("\033[1;34m"); // blue
            break;
        }
        case ORANGE: {
            printf("\033[1;35m"); // orange
            break;
        }
        case YELLOW: {
            printf("\033[1;33m"); // yellow
            break;
        }
        default: {
            printf("\e[41m");
            break;
        }
    }

    printf("X");
    printf("\e[0m");
}
