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
#include "odometrie.h"
#include "moteur.h"
#include "capteur.h"
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
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

/* USER CODE BEGIN PV */
RobotPose maPosition;
Moteur mot_gauche, mot_droit;

typedef struct {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
} IMU_Data;

#define OBSTACLE_DISTANCE_CM 20.0f
#define BACKUP_TIME_MS 500
#define TURN_TIME_MS 400
#define CONTROL_LOOP_MS 50
#define MAX_OBSTACLES 32

typedef struct {
    float x;
    float y;
    float distance_cm;
} ObstaclePoint;

ObstaclePoint obstacle_map[MAX_OBSTACLES];
uint32_t obstacle_count = 0;

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
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // Encodeur Gauche
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // Encodeur Droit
  ODOM_Init(&maPosition);

  // Configuration Moteur Gauche
  mot_gauche.htim = &htim4;
  mot_gauche.channel = TIM_CHANNEL_1;
  mot_gauche.dir_port = GPIOB;
  mot_gauche.dir_pin = GPIO_PIN_0;
  MOTEUR_Init(&mot_gauche);

  // Configuration Moteur Droit
  mot_droit.htim = &htim4;
  mot_droit.channel = TIM_CHANNEL_2;
  mot_droit.dir_port = GPIOB;
  mot_droit.dir_pin = GPIO_PIN_1;
  MOTEUR_Init(&mot_droit);

  CAPTEUR_t monCapteur = {GPIOA, GPIO_PIN_1, GPIOA, GPIO_PIN_2, &htim5};
  HAL_TIM_Base_Start(&htim5);


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
  float distance = 0;
  IMU_Data imu = {0};
  while (1)
  {
      ODOM_Update(&maPosition, &htim2, &htim3);
      IMU_Read(&imu);
      distance = CAPTEUR_Read_Distance(&monCapteur);

      Robot_RecordObstacle(&maPosition, distance);
      Robot_CheckBattery();

      if (distance <= OBSTACLE_DISTANCE_CM) {
          Robot_AvoidObstacle(&mot_gauche, &mot_droit);
      } else {
          int32_t base_speed = 50;
          float correction = -imu.gz * 0.5f; // compensation rotation selon gyroscope
          int32_t left_speed = (int32_t)fmaxf(-100, fminf(100, base_speed + correction));
          int32_t right_speed = (int32_t)fmaxf(-100, fminf(100, base_speed - correction));
          Robot_SetDifferential(left_speed, right_speed);
      }

      Robot_LogStatus(&maPosition, distance, &imu);

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
