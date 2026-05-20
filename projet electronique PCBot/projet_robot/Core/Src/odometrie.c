#include "odometrie.h"
#include <math.h>

// Variables "statiques" (propres à ce fichier) pour mémoriser l'état du robot
static float vitesse_lineaire = 0.0f; //Vitesse dans la direction où regarde le robot
static uint32_t last_time = 0; //Pour calculer le temps écoulé (dt)

//Initialise la position du robot et le chronomètre.

void ODOM_Init(RobotPose *pose){
    pose->x = 0.0f;
    pose->y = 0.0f;
    pose->theta = 0.0f;
    pose->last_pulse_gauche = 0;
    pose->last_pulse_droit = 0;
    vitesse_lineaire = 0.0f;
    last_time = HAL_GetTick(); //On commence à compter le temps à partir d'ici
}

/* Calcule la nouvelle position grâce à l'accéléromètre et au gyroscope.
 * @param acc_x  : Accélération mesurée sur l'axe X du robot (en m/s²)
 * @param gyro_z : Vitesse de rotation mesurée (en rad/s)
 */

void ODOM_Update_IMU(RobotPose *pose, float acc_x, float gyro_z){
    //Calcul du temps (dt)
    uint32_t current_time = HAL_GetTick();
    float dt = (float)(current_time - last_time) / 1000.0f; //On convertit les ms en secondes
    last_time = current_time;

    //Si le temps n'a pas coulé, on ne fait rien pour éviter des erreurs mathématiques
    if (dt <= 0) return;

    //MAJ de l'orientation (angle)
    //On intègre la vitesse de rotation pour obtenir l'angle total
    pose->theta += gyro_z * dt;

    //MAJ de la vitesse (intégration de l'accélération)
    //Vitesse = Vitesse précédente + (Accélération * temps)
    //On appelle ça la "méthode d'Euler"
    vitesse_lineaire += acc_x * dt;

    //MAJ de la position (X, Y)
    //On projette le mouvement sur la carte selon l'angle actuel du robot
    //Le déplacement est : vitesse * temps
    float deplacement = vitesse_lineaire * dt;

    pose->x += deplacement * cosf(pose->theta);
    pose->y += deplacement * sinf(pose->theta);
}


/* Nouvelle fonction ajoutée pour corriger l'erreur de compilation :
 * Calcule la position géométrique du robot via les ticks des encodeurs de roues (htimG et htimD)
 */
void ODOM_Update(RobotPose *pose, TIM_HandleTypeDef *htimG, TIM_HandleTypeDef *htimD) {
    // Lecture directe des compteurs de tics matériels des Timers de la STM32
    int32_t current_gauche = (int32_t)__HAL_TIM_GET_COUNTER(htimG);
    int32_t current_droit = (int32_t)__HAL_TIM_GET_COUNTER(htimD);

    // Calcul de la différence de tics (delta) depuis la dernière exécution
    int32_t delta_gauche = current_gauche - pose->last_pulse_gauche;
    int32_t delta_droit = current_droit - pose->last_pulse_droit;

    // Gestion de l'overflow du compteur (sur 16 bits)
    if (delta_gauche > 32767)  delta_gauche -= 65536;
    if (delta_gauche < -32768) delta_gauche += 65536;
    if (delta_droit > 32767)   delta_droit -= 65536;
    if (delta_droit < -32768)  delta_droit += 65536;

    // Sauvegarde pour le prochain cycle
    pose->last_pulse_gauche = current_gauche;
    pose->last_pulse_droit = current_droit;

    // Conversion des tics de rotation en mètres parcourus par chaque roue
    float dist_gauche = ((float)delta_gauche / CPR) * PI * DIAMETRE_ROUE;
    float dist_droit = ((float)delta_droit / CPR) * PI * DIAMETRE_ROUE;

    // Calcul du déplacement linéaire global et de la variation d'angle (orientation)
    float d_distance = (dist_droit + dist_gauche) / 2.0f;
    float d_theta = (dist_droit - dist_gauche) / ENTRAXE;

    // Mise à jour de la position globale stockée dans la structure RobotPose
    pose->theta += d_theta;
    pose->x += d_distance * cosf(pose->theta);
    pose->y += d_distance * sinf(pose->theta);
}
