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
        rotateZ(cube, act.index, act.cc);
    } else if (act.a == YZ) {
        rotateX(cube, act.index, act.cc);
    } else if (act.a == XZ) {
        rotateY(cube, act.index, act.cc);
    }
}

// Rotate a plane around the X-axis relative to the right.
// Indexing:
//    210
//    210
//    210
// 222210000012
// 222210000012
// 222210000012
//    210
//    210
//    210
void rotateX(cube_t *cube, int idx, int cc) {
    // Allocate temporary storage for a N-length segment.
    int temp[N];

    if (!cc) { // If clockwise.
        // Save front.
        for (int i = 0; i < N; i++)
            temp[i] = (*cube)[FRONT][(N - 1) - i][(N - 1) - idx];

        // Move bottom to front.
        for (int i = 0; i < N; i++)
            (*cube)[FRONT][(N - 1) - i][(N - 1) - idx] = (*cube)[BOTTOM][(N - 1) - i][(N - 1) - idx];

        // Move back to bottom.
        for (int i = 0; i < N; i++)
            (*cube)[BOTTOM][(N - 1) - i][(N - 1) - idx] = (*cube)[BACK][i][idx];

        // Move top to back.
        for (int i = 0; i < N; i++)
            (*cube)[BACK][i][idx] = (*cube)[TOP][(N - 1) - i][(N - 1) - idx];

        // Move front to top.
        for (int i = 0; i < N; i++)
            (*cube)[TOP][(N - 1) - i][(N - 1) - idx] = temp[i];
    } else { // If counter-clockwise.
        // Save front.
        for (int i = 0; i < N; i++)
            temp[i] = (*cube)[FRONT][i][(N - 1) - idx];

        // Move top to front
        for (int i = 0; i < N; i++)
            (*cube)[FRONT][i][(N - 1) - idx] = (*cube)[TOP][i][(N - 1) - idx];

        // Move back to top
        for (int i = 0; i < N; i++)
            (*cube)[TOP][i][(N - 1) - idx] = (*cube)[BACK][(N - 1) - i][idx];

        // Move bottom to back
        for (int i = 0; i < N; i++)
            (*cube)[BACK][(N - 1) - i][idx] = (*cube)[BOTTOM][i][(N - 1) - idx];

        // Move front to bottom
        for (int i = 0; i < N; i++)
            (*cube)[BOTTOM][i][(N - 1) - idx] = temp[i];
    }

    // Call rotate face if it's on an edge
    if (idx == 0) rotate_face(cube, RIGHT, cc);
    if (idx == N - 1) rotate_face(cube, LEFT, !cc);
}



// Rotate a plane around the Y-axis, relative to the top.
// Indexing:
//    000
//    000
//    000
// 000000000000
// 111111111111
// 222222222222
//    222
//    222
//    222
void rotateY(cube_t *cube, int idx, int cc) {
    int temp[N];

    if (!cc) {
        // Save front.
        for (int i = 0; i < N; i++)
            temp[i] = (*cube)[FRONT][idx][(N - 1) - i];

        // Move right to front.
        for (int i = 0; i < N; i++)
            (*cube)[FRONT][idx][(N - 1) - i] = (*cube)[RIGHT][idx][(N - 1) - i];

         // Move back to right.
        for (int i = 0; i < N; i++)
            (*cube)[RIGHT][idx][(N - 1) - i] = (*cube)[BACK][idx][(N - 1) - i];
        
        // Move left to back.
        for (int i = 0; i < N; i++)
            (*cube)[BACK][idx][(N - 1) - i] = (*cube)[LEFT][idx][(N - 1) - i];

        // Move front to left.
        for (int i = 0; i < N; i++)
            (*cube)[LEFT][idx][(N - 1) - i] = temp[i];
    } else {
        // Save front.
        for (int i = 0; i < N; i++)
            temp[i] = (*cube)[FRONT][idx][i];

        // Move left to front.
        for (int i = 0; i < N; i++)
            (*cube)[FRONT][idx][i] = (*cube)[LEFT][idx][i];

        // Move back to left.
        for (int i = 0; i < N; i++)
            (*cube)[LEFT][idx][i] = (*cube)[BACK][idx][i];

        // Move right to back.
        for (int i = 0; i < N; i++)
            (*cube)[BACK][idx][i] = (*cube)[RIGHT][idx][i];

        // Move front to right.
        for (int i = 0; i < N; i++)
            (*cube)[RIGHT][idx][i] = temp[i];
    }
    // Call rotate face if it's on an edge
    if (idx == 0) rotate_face(cube, TOP, cc);
    if (idx == N - 1) rotate_face(cube, BOTTOM, !cc);
}

// Rotate a plane around the Z-axis, relative to the front.
// Indexing:
//    222
//    111
//    000
// 210000012222
// 210000012222
// 210000012222
//    000
//    111
//    222
void rotateZ(cube_t* cube, int idx, int cc) {
    int temp[N];
    if (!cc) {
        // Save left.
        for (int i = 0; i < N; i++)
            temp[i] = (*cube)[LEFT][(N - 1) - i][(N - 1) - idx];

        // Move bottom to left.
        for (int i = 0; i < N; i++)
            (*cube)[LEFT][(N - 1) - i][(N - 1) - idx] = (*cube)[BOTTOM][idx][(N - 1) - i];

        // Move right to bottom.
        for (int i = 0; i < N; i++)
            (*cube)[BOTTOM][idx][(N - 1) - i] = (*cube)[RIGHT][i][idx];

        // Move top to right.
        for (int i = 0; i < N; i++)
            (*cube)[RIGHT][i][idx] = (*cube)[TOP][(N - 1) - idx][i];

        // Move left to top.
        for (int i = 0; i < N; i++)
            (*cube)[TOP][(N - 1) - idx][i] = temp[i];
    } else {
        // Save left.
        for (int i = 0; i < N; i++)
            temp[i] = (*cube)[LEFT][i][(N - 1) - idx];

        // Move top to left.
        for (int i = 0; i < N; i++)
            (*cube)[LEFT][i][(N - 1) - idx] = (*cube)[TOP][(N - 1) - idx][(N - 1) - i];

        // Move right to top.
        for (int i = 0; i < N; i++)
            (*cube)[TOP][(N - 1) - idx][(N - 1) - i] = (*cube)[RIGHT][(N - 1) - i][idx];

        // Move bottom to right.
        for (int i = 0; i < N; i++)
            (*cube)[RIGHT][(N - 1) - i][idx] = (*cube)[BOTTOM][idx][i];

        // move left to bottom.
        for (int i = 0; i < N; i++)
            (*cube)[BOTTOM][idx][i] = temp[i];
    }

    // Call rotate face if it's on an edge
    if (idx == 0) rotate_face(cube, FRONT, cc);
    if (idx == N - 1) rotate_face(cube, BACK, !cc);
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
    // Allocate space for a temporary NxN array.
    color_t temp_face[N][N];

    // Copy over original face
    memcpy(temp_face, (*cube)[side], N * N * sizeof(color_t));

    // Store each color to it's appropriate post-rotation location.s
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            (*cube)[side][i][j] = cc
                ? temp_face[j][(N - 1) - i]
                : temp_face[(N - 1) - j][i];
        }
    }
}

//TODO: replace with function to check hashed solution
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


void print_cube(cube_t* cube) {
    printf("\n");
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

//    012
//    345
//    678
// 012012012012
// 345345345345
// 678678678678
//    012
//    345
//    678

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
