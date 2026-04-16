/*
 * Ce fichier implémente un module de communication UART pour :
 * - initialiser l'UART utilisée par le module,
 * - formater l'état du robot (ID, position, orientation, distance),
 * - envoyer cet état vers une base sous forme de texte.
 */
#include "comm.h"   // Inclut les déclarations du module de communication.
#include <stdio.h>   // Inclut snprintf pour formater une chaîne de caractères.

static UART_HandleTypeDef *comm_huart;   // Pointeur UART conservé localement pour les envois.

void COMM_Init(UART_HandleTypeDef *huart) {   // Initialise le module avec l'UART à utiliser.
    comm_huart = huart;   // Mémorise le handle UART fourni pour les transmissions futures.
}   // Fin de l'initialisation du module de communication.

void COMM_SendToBase(const RobotPose *pose, float distance_cm) {   // Prépare et envoie l'état du robot.
    if (comm_huart == NULL) {   // Vérifie que l'UART a été initialisée avant d'envoyer.
        return;   // Quitte la fonction si aucune UART n'est disponible.
    }   // Fin de la vérification d'initialisation UART.

    char buffer[128];   // Crée un tampon pour contenir le message texte à transmettre.
    int len = snprintf(buffer, sizeof(buffer),   // Formate les données du robot dans le tampon.
                       "ID:%u X:%.2f Y:%.2f T:%.2f DIST:%.1fcm\r\n",   // Définit le format du message envoyé.
                       pose->id, pose->x, pose->y, pose->theta, distance_cm);   // Insère les valeurs réelles dans le format.

    if (len > 0) {   // Vérifie que snprintf a bien produit une chaîne valide.
        HAL_UART_Transmit(comm_huart, (uint8_t *)buffer, (uint16_t)len, 100);   // Envoie le message via UART avec un timeout de 100 ms.
    }   // Fin du bloc d'envoi UART.
}   // Fin de la fonction d'envoi vers la base.