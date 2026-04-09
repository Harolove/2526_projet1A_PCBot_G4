/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "odometrie.h"  // Gestion de la position du robot
#include "moteur.h"     // Contrôle des moteurs
#include "capteur.h"    // Capteur ultrasonique
#include "comm.h"       // Communication inter-robots et base
#include "map.h"        // Cartographie et fusion
#include "zone.h"       // Gestion des zones
#include <math.h>       // Fonctions mathématiques
#include <stdio.h>      // Entrées/sorties
#include <stdbool.h>    // Type booléen
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// Position actuelle du robot
RobotPose maPosition;
// Moteurs gauche et droit
Moteur mot_gauche, mot_droit;

// Structure pour données IMU (non utilisée actuellement)
typedef struct {
    float ax, ay, az;  // Accélérations
    float gx, gy, gz;  // Gyroscopes
} IMU_Data;

// Constantes pour l'évitement d'obstacles et contrôle
#define OBSTACLE_DISTANCE_CM 20.0f  // Distance seuil pour détecter un obstacle
#define BACKUP_TIME_MS 500          // Temps de recul en ms
#define TURN_TIME_MS 400            // Temps de rotation en ms
#define CONTROL_LOOP_MS 50          // Période de la boucle de contrôle
#define MAX_OBSTACLES 32            // Nombre max d'obstacles stockés

// Structure pour un point d'obstacle
typedef struct {
    float x, y;           // Position
    float distance_cm;    // Distance mesurée
} ObstaclePoint;

// Tableau des obstacles détectés
ObstaclePoint obstacle_map[MAX_OBSTACLES];
uint32_t obstacle_count = 0;

// Carte globale pour fusion
MapGrid global_map;
// Zone assignée au robot
Zone my_zone;
// Obstacles pour communication
CommObstacle comm_obstacles[MAX_OBSTACLES];
// Capteur ultrasonique
CAPTEUR_t monCapteur;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

static void Robot_Stop(void);
static void Robot_SetDifferential(int32_t left, int32_t right);
static void Robot_AvoidObstacle(Moteur *g, Moteur *d);
static void Robot_CheckBattery(void);
static void IMU_Read(IMU_Data *imu);
static void Robot_RecordObstacle(const RobotPose *pose, float distance_cm);
static void Robot_LogStatus(const RobotPose *pose, float distance_cm, const IMU_Data *imu);
static void Robot_SendToBase(void);
static void Robot_ProcessInterComm(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void Robot_Stop(void) {
    MOTEUR_SetVitesse(&mot_gauche, 0);
    MOTEUR_SetVitesse(&mot_droit, 0);
}

static void Robot_SetDifferential(int32_t left, int32_t right) {
    MOTEUR_SetVitesse(&mot_gauche, left);
    MOTEUR_SetVitesse(&mot_droit, right);
}

static void IMU_Read(IMU_Data *imu) {
    // Driver inertiel non disponible dans cette base; stub à compléter pour I2C/SPI
    imu->ax = 0.0f;
    imu->ay = 0.0f;
    imu->az = 0.0f;
    imu->gx = 0.0f;
    imu->gy = 0.0f;
    imu->gz = 0.0f;
}

static void Robot_CheckBattery(void) {
    // Stub de supervision batterie/USB : à remplacer par lecture ADC + GPIO USB
    float battery_voltage = 7.4f; // valeur simulée
    bool usb_connected = true;

    if (!usb_connected) {
        // Si pas de recharge USB et batterie faible, réduire la vitesse
        if (battery_voltage < 6.8f) {
            Robot_Stop();
            HAL_Delay(100);
            return;
        }
    }
    (void)battery_voltage;
    (void)usb_connected;
}

static void Robot_RecordObstacle(const RobotPose *pose, float distance_cm) {
    if (distance_cm < OBSTACLE_DISTANCE_CM && obstacle_count < MAX_OBSTACLES) {
        obstacle_map[obstacle_count].x = pose->x;
        obstacle_map[obstacle_count].y = pose->y;
        obstacle_map[obstacle_count].distance_cm = distance_cm;
        obstacle_count++;
    }
}

static void Robot_LogStatus(const RobotPose *pose, float distance_cm, const IMU_Data *imu) {
    // Émission à un réseau de robots ou debug
    // Format de message : "POSE x y theta dist imu_gx imu_gy imu_gz\r\n"
    char message[128];
    int len = snprintf(message, sizeof(message), "POSE:%.2f,%.2f,%.2f DIST:%.1fcm IMU:%.2f,%.2f,%.2f\r\n",
          pose->x, pose->y, pose->theta, distance_cm, imu->gx, imu->gy, imu->gz);

    (void)len;
    // Si port série disponible, appeler HAL_UART_Transmit(&huartX, (uint8_t*)message, len, 20);
}

static void Robot_SendToBase(void) {
    // Conversion des obstacles locaux en format communication
    for (uint32_t i = 0; i < obstacle_count; i++) {
        comm_obstacles[i].x = obstacle_map[i].x;
        comm_obstacles[i].y = obstacle_map[i].y;
        comm_obstacles[i].distance_cm = obstacle_map[i].distance_cm;
    }
    // Envoi à la station de base pour visualisation
    COMM_SendToBase(&maPosition, comm_obstacles, obstacle_count);
}

static void Robot_ProcessInterComm(void) {
    CommMessage msg;
    // Réception de messages inter-robots
    if (COMM_ReceiveMessage(&msg)) {
        if (msg.type == MSG_FUSION_DATA) {
            // Fusion des données de carte reçues
            MapGrid remote_map;
            // TODO: Désérialiser payload.fusion en remote_map
            MAP_FuseData(&global_map, &remote_map);
        }
        // Autres types de messages peuvent être traités ici
    }
}

static void Robot_AvoidObstacle(Moteur *g, Moteur *d) {
    // Arrêt + recul + rotation
    MOTEUR_SetVitesse(g, 0);
    MOTEUR_SetVitesse(d, 0);
    HAL_Delay(50);

    MOTEUR_SetVitesse(g, -40);
    MOTEUR_SetVitesse(d, -40);
    HAL_Delay(BACKUP_TIME_MS);

    MOTEUR_SetVitesse(g, 40);
    MOTEUR_SetVitesse(d, -40);
    HAL_Delay(TURN_TIME_MS);

    Robot_Stop();
    HAL_Delay(50);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();

  // Démarrage des encodeurs pour odométrie
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // Encodeur roue gauche
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // Encodeur roue droite
  // Initialisation odométrie
  ODOM_Init(&maPosition);
  maPosition.id = 1;  // ID unique du robot (modifier pour chaque robot)

  // Initialisation des modules ajoutés
  MAP_Init(&global_map);              // Carte globale vide
  ZONE_AssignFromID(maPosition.id, &my_zone);  // Assignation zone basée sur ID
  COMM_Init(&huart2);                 // Communication UART (inter-robots)

  // Configuration moteur gauche
  mot_gauche.htim = &htim4;
  mot_gauche.channel = TIM_CHANNEL_1;
  mot_gauche.dir_port = GPIOB;
  mot_gauche.dir_pin = GPIO_PIN_0;
  MOTEUR_Init(&mot_gauche);

  // Configuration moteur droit
  mot_droit.htim = &htim4;
  mot_droit.channel = TIM_CHANNEL_2;
  mot_droit.dir_port = GPIOB;
  mot_droit.dir_pin = GPIO_PIN_1;
  MOTEUR_Init(&mot_droit);

  // Configuration capteur ultrasonique
  monCapteur.trig_port = GPIOA;
  monCapteur.trig_pin = GPIO_PIN_1;
  monCapteur.echo_port = GPIOA;
  monCapteur.echo_pin = GPIO_PIN_2;
  monCapteur.htim = &htim6;
  HAL_TIM_Base_Start(&htim6);


#ifdef UNIT_TEST
  // Test auto-exécuté en mode simulation
  {
      bool all_ok = true;
      RobotPose p;
      ODOM_Init(&p);

      ODOM_UpdateFromCounts(&p, 1024, 1024);
      if (fabsf(p.x - (PI * DIAMETRE_ROUE)) > 1e-4f) all_ok = false;

      float d = CAPTEUR_ConvertTimeToDistance(1000);
      if (fabsf(d - (1000.0f * 0.034f / 2.0f)) > 1e-6f) all_ok = false;

      if (!all_ok) {
          // Indicate error (boucle infinie pour debug)
          while (1) {
              HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
              HAL_Delay(200);
          }
      }
      // succès : clignoter plus lentement
      for (int i = 0; i < 10; i++) {
          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
          HAL_Delay(500);
      }
      while (1) {
          // fini
      }
  }
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  float distance = 0;  // Distance mesurée par le capteur
  IMU_Data imu = {0};  // Données IMU (non utilisées)
  while (1)
  {
      // Mise à jour de la position via odométrie
      ODOM_Update(&maPosition, &htim2, &htim3);
      // Lecture IMU (stub)
      IMU_Read(&imu);
      // Mesure distance avec capteur ultrasonique
      distance = CAPTEUR_Read_Distance(&monCapteur);

      // Enregistrement obstacle et ajout à la carte
      Robot_RecordObstacle(&maPosition, distance);
      MAP_AddObstacle(&global_map, maPosition.x, maPosition.y);
      // Vérification batterie
      Robot_CheckBattery();
      // Traitement communications inter-robots
      Robot_ProcessInterComm();

      // Évitement d'obstacle ou mouvement normal
      if (distance <= OBSTACLE_DISTANCE_CM) {
          Robot_AvoidObstacle(&mot_gauche, &mot_droit);
      } else {
          int32_t base_speed = 50;  // Vitesse de base
          float correction = -imu.gz * 0.5f;  // Compensation rotation via gyroscope
          int32_t left_speed = (int32_t)fmaxf(-100, fminf(100, base_speed + correction));
          int32_t right_speed = (int32_t)fmaxf(-100, fminf(100, base_speed - correction));
          // Ajustement pour rester dans la zone assignée
          ZONE_AdjustMovement(&my_zone, &left_speed, &right_speed, &maPosition);
          Robot_SetDifferential(left_speed, right_speed);
      }

      // Log et envoi à la base
      Robot_LogStatus(&maPosition, distance, &imu);
      Robot_SendToBase();

      // Délai boucle de contrôle
      HAL_Delay(CONTROL_LOOP_MS);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_TIM2_Init(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  __HAL_RCC_TIM2_CLK_ENABLE();

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;

  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM3_Init(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  __HAL_RCC_TIM3_CLK_ENABLE();

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;

  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM4_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  __HAL_RCC_TIM4_CLK_ENABLE();

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 159;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  __HAL_RCC_TIM6_CLK_ENABLE();

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 159;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  __HAL_RCC_USART2_CLK_ENABLE();

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
