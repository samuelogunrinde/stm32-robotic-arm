#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define L1 100.0f   // Shoulder link (mm)
#define L2 100.0f   // Elbow link (mm)
#define BASE_HEIGHT 50.0f // Base Height (mm)

#define RAD_TO_DEG(x) ((x) * 180.0f / 3.14159265f)

// Manipulator offsets calibration
#define BASE_OFFSET      50.0f
#define SHOULDER_OFFSET  30.0f
#define ELBOW_OFFSET     30.0f

// Solve Inverse Kinematics
bool IK_Solve(float x, float y, float z, uint16_t angles[4]) {

    float theta0, theta1, theta2;

    // Distance from base axis
    float r = sqrtf(x*x + y*y);

    // Height relative to shoulder joint
    float z_eff = z - BASE_HEIGHT;

    // Geometry constraint
    float D = (r*r + z_eff*z_eff - L1*L1 - L2*L2) / (2 * L1 * L2);

    // Check reachability
    if (D > 1.0f || D < -1.0f) {
        return false;  // unreachable
    }

    // Base rotation
    theta0 = atan2f(y, x);

    // Elbow angle
    theta2 = atan2f(sqrtf(1 - D*D), D);

    // Shoulder angle
    theta1 = atan2f(z_eff, r) - atan2f(L2*sinf(theta2), L1 + L2*cosf(theta2));

    // radians to degrees
    angles[0] = (uint16_t)(RAD_TO_DEG(theta0) + BASE_OFFSET);
    angles[1] = (uint16_t)(RAD_TO_DEG(theta1) + SHOULDER_OFFSET);
    angles[2] = (uint16_t)(RAD_TO_DEG(theta2) + ELBOW_OFFSET);
    angles[3] = 0;   // gripper

    return true;
}
