

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
