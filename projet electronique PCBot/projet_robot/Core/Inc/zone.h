#ifndef INC_ZONE_H_
#define INC_ZONE_H_

#include <stdint.h>
#include <stdbool.h>
#include "odometrie.h"

typedef struct {
    float x_min, y_min;
    float x_max, y_max;
} Zone;

typedef uint8_t RobotID;

void ZONE_AssignFromID(RobotID id, Zone *zone);
bool ZONE_IsInZone(const RobotPose *pose, const Zone *zone);
void ZONE_AdjustMovement(const Zone *zone, int32_t *left_speed, int32_t *right_speed, const RobotPose *pose);

#endif /* INC_ZONE_H_ */