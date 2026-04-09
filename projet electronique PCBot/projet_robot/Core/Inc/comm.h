#ifndef INC_COMM_H_
#define INC_COMM_H_

#include "stm32g4xx_hal.h"

#include "odometrie.h"

/* Communication UART très simple pour afficher l'état du robot. */
void COMM_Init(UART_HandleTypeDef *huart);
void COMM_SendToBase(const RobotPose *pose, float distance_cm);

#endif /* INC_COMM_H_ */