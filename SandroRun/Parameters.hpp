#ifndef PARAMETERS
#define PARAMETERS

// Camera settings
#define FOV_Y glm::radians(45.0f)
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f
#define CAM_HEIGHT 1.0f
#define CAM_DIST 5.0f
#define MIN_PITCH glm::radians(10.0f)
#define MAX_PITCH glm::radians(80.0f)

// Moto settings
#define ROT_SPEED glm::radians(120.0f)
#define Z_SPEED 3.0f
#define X_SPEED 21.0f
#define MIN_MOTO_ROLL (float) (-M_PI / 4.0f)
#define MAX_MOTO_ROLL (-MIN_MOTO_ROLL)
#define MIN_MOTO_PITCH 0.0f
#define MAX_MOTO_PITCH (float) (M_PI / 4.0f)
#define MOTO_ROLL_SPEED 100
#define MOTO_PITCH_SPEED 150
#define LAMBDA 10.0f

// Cars settings
#define NUM_CAR1_INSTANCES 4
#define RIGHT_LANE 6.5f
#define CENTER_RIGHT_LANE 2.2f
#define CENTER_LEFT_LANE -CENTER_RIGHT_LANE
#define LEFT_LANE -RIGHT_LANE
#define MAX_CAR_SPEED 30
#define MIN_CAR_SPEED 15

const float CAR1_STARTING_POSITIONS[NUM_CAR1_INSTANCES][2] = {{CENTER_LEFT_LANE, -150},
                                                              {RIGHT_LANE, -10},
                                                              {CENTER_RIGHT_LANE, -80},
                                                              {LEFT_LANE, -200}};
const bool CAR1_STARTING_DIRECTIONS[NUM_CAR1_INSTANCES] = {false, true, true, false};
const float CAR1_STARTING_VELOCITIES[NUM_CAR1_INSTANCES] = {-10.0f, 13.0f, 17.0f, -15.0f};

// World settings
#define TERRAIN_LENGTH 120

#endif