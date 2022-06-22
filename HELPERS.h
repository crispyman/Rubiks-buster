#ifndef RUBIKS_BUSTER_HELPERS_H
#define RUBIKS_BUSTER_HELPERS_H
    #include <stdbool.h>

    #define SIDES 6
    #define N 3
    #define DEBUG 0
    #define N_SCRAMBLES 6
    #define MAX_SOLUTION_LENGTH 5
    #define MAX_STACK_SIZE 20

    /* ----- Type Definitions ----- */

    // Color definition.
    typedef enum __attribute__ ((__packed__)) {NONE, WHITE, GREEN, RED, BLUE, ORANGE, YELLOW} color_t;

    // Side definition.
    typedef enum {TOP, LEFT, FRONT, RIGHT, BACK, BOTTOM} side_t;

    // Rotation action definition.
    typedef enum {X, Y, Z} axis_t;
    typedef struct {
        axis_t a;
        int index;
        int cc;
    } rotate_action_t;

    // N x N x 6 rubix cube definition definition.
    typedef color_t cube_t[SIDES][N][N];

    typedef struct {
        rotate_action_t * steps;
        int length;
    } solution_t;

    typedef struct {
        rotate_action_t steps[MAX_STACK_SIZE];
        int top;
    } action_stack_t;

    /* ----- Prototypes ----- */

    // Borrowed from Dr. Norris, Thank You!
    void unixError(char * msg);

    // Rotate functions.
    void rotate(cube_t* cube, rotate_action_t act);
    void rotateX(cube_t* cube,int row, int cc);
    void rotateY(cube_t* cube,int col, int cc);
    void rotateZ(cube_t* cube,int col, int cc);
    void rotate_face(cube_t* cube, side_t side, int cc);

    // Verification functions.
    void verifyValid(cube_t* cube);
    bool checkSolved(cube_t* cube);

    // Display functions.
    void print_cube(cube_t* cube);
    void print_color(color_t color);

    // Action stack
    void stack_init(action_stack_t* stack);
    bool stack_push(action_stack_t* stack, rotate_action_t act);
    bool check_action_backtrack(action_stack_t* stack, rotate_action_t* act);
    bool check_action_loop(action_stack_t* stack, rotate_action_t* act);

#endif //RUBIKS_BUSTER_HELPERS_H
