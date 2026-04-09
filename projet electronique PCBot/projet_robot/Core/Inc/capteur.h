#ifndef INC_CAPTEUR_H_
#define INC_CAPTEUR_H_

#include "stm32g4xx_hal.h"

typedef struct {
    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;
    GPIO_TypeDef *echo_port;
    uint16_t echo_pin;
    TIM_HandleTypeDef *htim;
} CAPTEUR_t;

float CAPTEUR_Read_Distance(CAPTEUR_t *sensor);

#endif /* INC_CAPTEUR_H_ */
