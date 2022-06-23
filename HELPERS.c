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
    if (act.a == Z) {
        rotateZ(cube, act.index, act.cc);
    } else if (act.a == X) {
        rotateX(cube, act.index, act.cc);
    } else if (act.a == Y) {
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

// Just ensures that each spot has a valid color in it, does not check edges
void verifyValid(cube_t* cube) {
    for (int i = 0; i < SIDES; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (!((*cube)[i][j][k] > NONE && (*cube)[i][j][k] <= YELLOW)) {
                    //borrowed from an OS lab, Thanks Dr. Norris!
                    fprintf(stderr, "\033[91mERROR: Side: %d x: %d y: %d Value: %d\033[0m\n", i, j, k, (*cube)[i][j][k]);
                    exit(1);
                }
            }
        }
    }
}

// Check to see if each side contains only a single color.
bool checkSolved(cube_t* cube) {
    for (int i = 0; i < SIDES; i++) {
        color_t side_color = NONE;
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (side_color == NONE) side_color = (*cube)[i][j][k];
                else if (side_color != (*cube)[i][j][k]) return false;
            }
        }
    }
    return true;
}

// Print a flattened version of a NxN cube.
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

// Print an "X" of the selected color. (There was no orange so it is replaced
// with magenta.)
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
            printf("\e[41m"); // Fill Red (error)
            break;
        }
    }

    printf("X");
    printf("\e[0m");
}

/* ----- STACK STUFF ----- */

// Given a stack pointer, set the top to 0.
void stack_init(action_stack_t* stack) {
    stack->top = 0;
}

// Check if a action is actually progress and if it is, push it onto the action
// stack.
bool stack_push(action_stack_t* stack, rotate_action_t act) {
    // Check if stack is full.
    if (stack->top == MAX_STACK_SIZE) return false;

    // If the new action would undo the last one, return false.
    if (check_action_backtrack(stack, &act)) return false;

    // If the new action would cause a 360 degree loop, return false.
    if (check_action_loop(stack, &act)) return false;

    // Push the action onto the stack.
    stack->steps[stack->top] = act;
    stack->top++;
    return true;
}

// Tell whether a new action would cause the previous action to be undone.
bool check_action_backtrack(action_stack_t* stack, rotate_action_t* act) {
    // If the stack is empty, return false.
    if (stack->top == 0) return false;

    // If the previous action and the new one are working on different axes,
    // return false.
    if (stack->steps[stack->top - 1].a != act->a) return false;

    // If the previous action and the new one are working on different indexes,
    // return false.
    if (stack->steps[stack->top - 1].index != act->index) return false;

    // If the previous action and the new one are rotating in the same
    // direction, return false.
    if (stack->steps[stack->top - 1].index != act->index) return false;

    // All requirements have been made to consider the new action a backtrack,
    // return true.
    return true;
}

// Tell whether a new action would cause one roationn 4 times in a row, leading
// leading back to the same starting state.
bool check_action_loop(action_stack_t* stack, rotate_action_t* act) {
    // If the stack has less than 3 actions, return false.
    if (stack->top < 4) return false;

    // Check if each of the previous 3 rotations is the same as the new one.
    for (int i = 0; i < 3; i++) {
        if (stack->steps[stack->top - 1 - i].a != act->a) return false;
        if (stack->steps[stack->top - 1 - i].index != act->index) return false;
        if (stack->steps[stack->top - 1 - i].cc != act->cc) return false;
    }

    // All requirements have been made to consider the new action a loop,
    // return true.
    return true;
}