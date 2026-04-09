// Pour estimer la position (x, y) et l'orientation (θ) du robot, en comptant les tours de roue
//(grâce aux encodeurs)

#include "odometrie.h"

void ODOM_Init(RobotPose *pose) {
    pose->x = 0.0f;
    pose->y = 0.0f;
    pose->theta = 0.0f;
    pose->last_pulse_gauche = 0;
    pose->last_pulse_droit = 0;
}

static void ODOM_UpdateFromDiff(RobotPose *pose, int32_t diff_g, int32_t diff_d) {
    float distance_gauche = (float)diff_g * (PI * DIAMETRE_ROUE) / CPR;
    float distance_droite = (float)diff_d * (PI * DIAMETRE_ROUE) / CPR;
    float distance = (distance_droite + distance_gauche) / 2.0f;
    float rotation = (distance_droite - distance_gauche) / ENTRAXE;

    pose->x += distance * cosf(pose->theta);
    pose->y += distance * sinf(pose->theta);
    pose->theta += rotation;
}

void ODOM_Update(RobotPose *pose, TIM_HandleTypeDef *htimG, TIM_HandleTypeDef *htimD) {
    int32_t current_gauche = (int32_t)__HAL_TIM_GET_COUNTER(htimG);
    int32_t current_droit = (int32_t)__HAL_TIM_GET_COUNTER(htimD);

    ODOM_UpdateFromDiff(pose,
                        current_gauche - pose->last_pulse_gauche,
                        current_droit - pose->last_pulse_droit);

    pose->last_pulse_gauche = current_gauche;
    pose->last_pulse_droit = current_droit;
}

