#include "comm.h"
#include <stdio.h>

static UART_HandleTypeDef *comm_huart;

void COMM_Init(UART_HandleTypeDef *huart) {
    comm_huart = huart;
}

void COMM_SendToBase(const RobotPose *pose, float distance_cm) {
    if (comm_huart == NULL) {
        return;
    }

    char buffer[128];
    int len = snprintf(buffer, sizeof(buffer),
                       "ID:%u X:%.2f Y:%.2f T:%.2f DIST:%.1fcm\r\n",
                       pose->id, pose->x, pose->y, pose->theta, distance_cm);

    if (len > 0) {
        HAL_UART_Transmit(comm_huart, (uint8_t *)buffer, (uint16_t)len, 100);
    }
}