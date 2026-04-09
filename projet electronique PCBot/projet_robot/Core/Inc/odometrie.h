#ifndef INC_ODOMETRIE_H_
#define INC_ODOMETRIE_H_

#include "stm32g4xx_hal.h"
#include <math.h>

#define PI 3.1415926535f
#define DIAMETRE_ROUE 0.065f
#define CPR 1024.0f
#define ENTRAXE 0.15f

typedef struct {
    float x;
    float y;
    float theta;
    int32_t last_pulse_gauche;
    int32_t last_pulse_droit;
    uint8_t id;
} RobotPose;

void ODOM_Init(RobotPose *pose);
void ODOM_Update(RobotPose *pose, TIM_HandleTypeDef *htimG, TIM_HandleTypeDef *htimD);

#endif /* INC_ODOMETRIE_H_ */
