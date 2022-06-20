#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include "HELPERS.h"

// Picks which rotate function to call
void rotate(cube_t *cube, rotate_action_t act) {
    if (act.a == XY) {
        rotateX(cube, act.index, act.cc);
    } else if (act.a == YZ) {
        rotateY(cube, act.index, act.cc);
    } else if (act.a == XZ) {
        rotateZ(cube, act.index, act.cc);
    }

}
///NOTES ON ALL: For reference for clockwise or counterclockwise I use the perspective of front, right, or top, like in the images
/// Look at cycles in HELPERS.h to see clockwise cycles
/*
*
* I used memcpy for no reason except speed.
*
*
*    +-----------+
*   /           /|
*  /           / |
* +-----------+  |
* |           |  |
* |           |  +
* |           | /
* | ◀-------▶ |/
* +-----------+
*/
void rotateX(cube_t *cube, int row, int cc) {
    int temp[N];

    if (!cc) {
        // Copies start cube into temp space starting with first side (FRONT)
        memcpy(temp, &(*cube)[cycleX[0]][row], N * sizeof(int));
        // Copies each row back to the previous side
        for (int i = 0; i < 3; i++) {
            memcpy(&(*cube)[cycleX[i]][row], &(*cube)[cycleX[i + 1]][row], N * sizeof(int));
        }
        // Copies temp back into last row (LEFT)
        memcpy(&(*cube)[cycleX[3]][row], temp, N * sizeof(int));
    } else {
        // Copies start cube into temp space starting with last side (LEFT)
        memcpy(temp, &cube[cycleX[3]][row], N * sizeof(int));
        // Copies each row to the next side
        for (int i = 3; i > 0; i--) {
            memcpy(&(*cube)[cycleX[i - 1]][row], &(*cube)[cycleX[i]][row], N * sizeof(int));
        }
        // Copies temp back into First row (Front)
        memcpy(&(*cube)[cycleX[0]][row], temp, N * sizeof(int));
    }

    // Call rotate face if it's on an edge
    if (row == 0) {
        rotate_face(cube, TOP, cc);
    } else if (row == N - 1) {
        rotate_face(cube, BOTTOM, cc);

    }
}

/*
 *    +-----------+
 *   /           /|
 *  /           / |
 * +-----------+  |
 * |    ▲      |  |
 * |    |      |  +
 * |    |      | /
 * |    ▼      |/
 * +-----------+
 */
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


/*
 *
 *    +-----------+
 *   /           /|
 *  / ◀-------▶ / |
 * +-----------+▲ |
 * |           || |
 * |           || +
 * |           |▼/
 * |           |/
 * +-----------+
 */

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
        //TODO: Isn't it columns on the sides the rows on Top?
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
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                assert((*cube)[i][j][k] > NONE && (*cube)[i][j][k] <= YELLOW);
            }
        }
    }
}


/*
 * Helper for the rotate functions, rotates neighboring side if row getting rotated
 * is on an edge.
 */
void rotate_face(cube_t *cube, int side, int cc) {
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

//bool checkSolved(cube_t* cube) {
//    int front_color = NONE;
//    int back_color = NONE;
//    int left_color = NONE;
//    int right_color = NONE;
//    int top_color = NONE;
//    int bottom_color = NONE;
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            for (int k = 0; k < N; k++) {
//                if (i == 0) {
//                    if (top_color == NONE)
//                        top_color = (*cube)[i][j][k][TOP];
//                    else if (top_color != (*cube)[i][j][k][TOP])
//                        return false;
//                }
//                if (j == 0) {
//                    if (left_color == NONE)
//                        left_color = (*cube)[i][j][k][LEFT];
//                    else if (left_color != (*cube)[i][j][k][LEFT])
//                        return false;
//                }
//                if (k == 0){
//                    if (front_color == NONE)
//                        front_color = (*cube)[i][j][k][FRONT];
//                    else if (front_color != (*cube)[i][j][k][FRONT])
//                        return false;
//                }
//                if (j == N - 1){
//                    if (right_color == NONE)
//                        right_color = (*cube)[i][j][k][RIGHT];
//                    else if (right_color != (*cube)[i][j][k][RIGHT])
//                        return false;
//                }
//                if (k == N - 1){
//                    if (back_color == NONE)
//                        back_color = (*cube)[i][j][k][BACK];
//                    else if (back_color != (*cube)[i][j][k][BACK])
//                        return false;
//                }
//                if (i == N - 1){
//                    if (bottom_color == NONE)
//                        bottom_color = (*cube)[i][j][k][BOTTOM];
//                    else if (bottom_color != (*cube)[i][j][k][BOTTOM])
//                        return false;
//                }
//            }
//        }
//    }
//    return true;
//}
// Given a populated plane, rotate it 90% either clockwise or counter clockwise.
