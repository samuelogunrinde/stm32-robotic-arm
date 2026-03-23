// This file contains the function prototype for the inverse_kinematics.c file

#define GRIP_OPEN   80
#define GRIP_CLOSE 170

typedef struct {
    float x;
    float y;
    float z;
} Position;

// predefined manipulator positions
Position PICK  = {150,  50, 30};
Position LIFT  = {150,  50, 80};
Position PLACE = {100, -50, 30};
Position HOME  = {0,   0, 0};

// function prototype
bool IK_Solve(float x, float y, float z, uint16_t angles[4]);
