// This file contains the function prototype for the inverse_kinematics.c file

// Calibrated gripper positions
#define GRIP_OPEN   110
#define GRIP_HOLD	150
#define GRIP_CLOSE 167

typedef struct {
    float x;
    float y;
    float z;
} Position;

// Calibrated manipulator positions
Position PICK  = {-15,  85, 100};
Position LIFT  = {30,  170,  85};
Position PLACE = {76,  -39, 100};
Position HOME  = {30,  170,  85};

// function prototype
bool IK_Solve(float x, float y, float z, uint16_t angles[4]);
