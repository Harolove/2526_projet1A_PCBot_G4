#include "zone.h"

void ZONE_AssignFromID(RobotID id, Zone *zone) {
    switch (id % 4) {
        case 0:
            zone->x_min = -5.0f;
            zone->x_max = 0.0f;
            zone->y_min = 0.0f;
            zone->y_max = 5.0f;
            break;
        case 1:
            zone->x_min = 0.0f;
            zone->x_max = 5.0f;
            zone->y_min = 0.0f;
            zone->y_max = 5.0f;
            break;
        case 2:
            zone->x_min = -5.0f;
            zone->x_max = 0.0f;
            zone->y_min = -5.0f;
            zone->y_max = 0.0f;
            break;
        default:
            zone->x_min = 0.0f;
            zone->x_max = 5.0f;
            zone->y_min = -5.0f;
            zone->y_max = 0.0f;
            break;
    }
}

bool ZONE_IsInZone(const RobotPose *pose, const Zone *zone) {
    if (pose->x < zone->x_min) {
        return false;
    }
    if (pose->x > zone->x_max) {
        return false;
    }
    if (pose->y < zone->y_min) {
        return false;
    }
    if (pose->y > zone->y_max) {
        return false;
    }

    return true;
}

void ZONE_AdjustMovement(const Zone *zone, int32_t *left_speed, int32_t *right_speed, const RobotPose *pose) {
    if (!ZONE_IsInZone(pose, zone)) {
        *left_speed /= 2;
        *right_speed /= 2;

        if (pose->x < zone->x_min) {
            *right_speed += 10;
        } else if (pose->x > zone->x_max) {
            *left_speed += 10;
        }

        if (pose->y < zone->y_min) {
            *right_speed += 10;
        } else if (pose->y > zone->y_max) {
            *left_speed += 10;
        }
    }
}