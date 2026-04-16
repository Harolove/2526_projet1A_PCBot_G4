/*
 * Ce fichier d'en-tête déclare l'interface du module de communication UART :
 * - initialisation de l'UART utilisée,
 * - envoi de l'état du robot vers la base.
 */
#ifndef INC_COMM_H_   // Vérifie si la garde d'inclusion n'est pas encore définie.
#define INC_COMM_H_   // Définit la garde d'inclusion pour éviter les inclusions multiples.

#include "stm32g4xx_hal.h"   // Rend disponible le type UART_HandleTypeDef de la HAL STM32.

#include "odometrie.h"   // Rend disponible le type RobotPose utilisé dans les prototypes.

/* Communication UART très simple pour afficher l'état du robot. */   // Résume le rôle du module.
void COMM_Init(UART_HandleTypeDef *huart);   // Déclare l'initialisation du module avec une UART donnée.
void COMM_SendToBase(const RobotPose *pose, float distance_cm);   // Déclare l'envoi de la pose et de la distance vers la base.

#endif /* INC_COMM_H_ */   // Termine la garde d'inclusion du fichier d'en-tête.