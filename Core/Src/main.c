/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "ssd1306.h"
#include "ssd1306_tests.h"
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
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

IWDG_HandleTypeDef hiwdg;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
int display = 0;
int i = 0, pastI = 0;
int screen = 8;
float volt = 0;
float voltSet = 12.7;
float pomiar1 = 0;
int ok = 0;
int sportState = 0;
int sportSet = 0;
int32_t counter = 0;
int32_t prevCounter = 0;
int pressed = 0;
int lock = 0;
int step = 0;
int temp;
int sekAdj = 0;
int minAdj = 0;
char voltage[6];
char voltageSet[6];
char contrastLcd[4];
char sleepLcd[4];
char timeSek[2];
char timeMin[2];
char timeSekGth[2];
char timeMinGth[2];
char timeSetSek[2];
char timeSetMin[2];
char timeSetSekGth[2];
char timeSetMinGth[2];
uint32_t value[3];
uint32_t milis = 0;
uint32_t prevmilis = 0;
uint32_t prevmilis2 = 0;
uint32_t prevmilis3 = 0;
uint32_t prevmilisBckl = 0;
uint32_t sekmilis = 0;
uint32_t prevmilisGth = 0;
uint32_t sekmilisGth = 0;
uint32_t blinkMilis1 = 0;
uint32_t blinkMilis2 = 0;
uint32_t voltmilis = 0;
uint16_t sek = 10;
uint16_t min = 1;
uint16_t sekGth = 40;
uint16_t minGth = 0;
uint8_t turbotimerOn = 0;
uint8_t turbotimerLock = 0;
int16_t count = 0;
int16_t prevCount = 0;
int8_t sekSet = 40;
int8_t minSet = 1;
int8_t sekSetGth = 40;
int8_t minSetGth = 0;
int charge = 0;
int chargeSet = 0;
int chargeSetLock = 0;
int voltageSetLock = 0;
int setLock = 0;
int reczny = 1;
int trigger = 0;
int enter = 0;
int gthLock = 0;
int contrast = 80;
int contrastSet = 0;
int sleep = 10;
int sleepSet = 0;
int kierunkowskaz = 0;
int blinkCount = 0;
int blinkLock = 0;
int carLocked = 0;
int carUnLocked = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_IWDG_Init(void);
/* USER CODE BEGIN PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) { // Jeżeli przerwanie pochodzi od timera 2
//	  		HAL_GPIO_TogglePin(GPIOA, LD4_Pin );
//	  	HAL_ADC_Start_DMA(&hadc1, dmaBuffer, 8); // Start ADC in DMA mode and declare the buffer where store the results
		milis++;
//		timeOff = timeTrigger2 - timeTrigger1 ;
		if (turbotimerOn == 1 && turbotimerLock == 0) {
			HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_RESET);
			if (milis - sekmilis >= 10) {
//				if (min == 0 && sek == 0) {
//					turbotimerOn = 0;
//				}
				sek--;
				sekmilis = milis;
			}

			if (milis - prevmilis >= 1) {
				if (sek == 65535) {
					sek = 59;
					min--;
				}
				if (min == 65535) {
					min = 0;
				}

				prevmilis = milis;
			}
		}
		if (volt < voltSet && gthLock == 0) {
			HAL_GPIO_WritePin(GPIOB, GthRelay_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GthLamp1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLamp2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLicznik_Pin, GPIO_PIN_SET);
			if (milis - sekmilisGth >= 10) {
				sekGth--;
				sekmilisGth = milis;
			}
			if (milis - prevmilisGth >= 1) {
				if (sekGth == 65535) {
					sekGth = 59;
					minGth--;
				}
				if (minGth == 65535) {
					minGth = 0;
				}

				prevmilisGth = milis;
			}
		}
		if (min == 0 && sek == 0) {
			turbotimerOn = 0;
			turbotimerLock = 1;
		} else {
			if (milis - sekmilis == 10) {
				prevmilis = milis;
				sekmilis = milis;
			}
		}
		if (minGth == 0 && sekGth == 0) {
			gthLock = 1;
			HAL_GPIO_WritePin(GPIOB, GthRelay_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLamp1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLamp2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLicznik_Pin, GPIO_PIN_SET);
		} else {
			if (milis - sekmilis == 10) {
				prevmilisGth = milis;
				sekmilisGth = milis;
			}
		}
		if (milis - blinkMilis1 >= 1) {
			if (kierunkowskaz == 0 && blinkLock == 0) {
				blinkCount++;
				blinkLock = 1;
				blinkMilis2 = milis;
			}
			if (kierunkowskaz == 1) {
				blinkLock = 0;
			}
			blinkMilis1 = milis;
//			blinkMilis2 = milis;
		}
		if (milis - blinkMilis2 >= 10) {
			if (kierunkowskaz == 1) {
				blinkCount = 0;
			}
			blinkMilis2 = milis;
		}
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//uint32_t counter = 0;
int speed = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	counter = __HAL_TIM_GET_COUNTER(&htim3);
	if (screen == 1 || screen == 6) {
		if (chargeSet == 0 && contrastSet == 0 && voltageSetLock == 0
				&& sleepSet == 0) {
			if (screen == 1) {
				if (counter - prevCounter == -4) {
					count--;
					prevCounter = counter;
					if (count < 0) {
						count = 0;
					}
				}
				if (counter - prevCounter == 4) {
					count++;
					prevCounter = counter;
					if (count > 5) {
						count = 5;
					}
				}
			}
			if (screen == 6) {
				if (counter - prevCounter == -4) {
					count--;
					prevCounter = counter;
					if (count < 0) {
						count = 0;
					}
				}
				if (counter - prevCounter == 4) {
					count++;
					prevCounter = counter;
					if (count > 4) {
						count = 4;
					}
				}
			}
		}
		if (chargeSet == 1) {
			if (counter - prevCounter == -4) {
				count--;
				prevCounter = counter;
				if (count <= 0) {
					charge = 0;
					count = 0;
				}
			}
			if (counter - prevCounter == 4) {
				count++;
				prevCounter = counter;
				if (count >= 1) {
					charge = 1;
					count = 1;
				}
			}
		}
		if (contrastSet == 1) {
			if (counter - prevCounter == -4) {
				count--;
				contrast = count;
				prevCounter = counter;
				if (count <= 10) {
					contrast = 10;
					count = 10;
				}
			}
			if (counter - prevCounter == 4) {
				count++;
				contrast = count;
				prevCounter = counter;
				if (count >= 99) {
					contrast = 99;
					count = 99;
				}
			}
		}
		if (voltageSetLock == 1) {
			if (counter - prevCounter == -4) {
				count--;
				voltSet = (count / 10.0);
				prevCounter = counter;
				if (count <= 100) {
					voltSet = 10.0;
					count = 100;
				}
			}
			if (counter - prevCounter == 4) {
				count++;
				voltSet = (count / 10.0);
				prevCounter = counter;
				if (count >= 140) {
					voltSet = 14.0;
					count = 140;
				}
			}
		}
		if (sleepSet == 1) {
			if (counter - prevCounter == -4) {
				count--;
				sleep = count;
				prevCounter = counter;
				if (count <= 10) {
					sleep = 10;
					count = 10;
				}
			}
			if (counter - prevCounter == 4) {
				count++;
				sleep = count;
				prevCounter = counter;
				if (count >= 90) {
					sleep = 90;
					count = 90;
				}
			}
		}
		if (counter >= 65530) {
			TIM3->CNT = 0;
			counter = 0;
		}
	}
	if (screen == 2 || screen == 3) {
		if (counter - prevCounter == -4) {
			count--;
			prevCounter = counter;
		}
		if (counter - prevCounter == 4) {
			count++;
			prevCounter = counter;
		}
	}

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
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
	ssd1306_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		HAL_IWDG_Refresh(&hiwdg);
		ssd1306_SetContrast(contrast);
		void dots() {  //// +13
			if (screen == 0 || screen == 2 || screen == 3 || screen == 4
					|| screen == 7) {
				ssd1306_DrawRectangle(0, 12, 120, 44, White);
				if (screen == 2 || screen == 3) {
					if (minAdj == 2) {
						ssd1306_DrawRectangle(32, 15, 57, 33, Black);
						ssd1306_DrawRectangle(69, 15, 92, 32, Black);
						ssd1306_DrawRectangle(45, 52, 80, 62, White);
					}
					if (minAdj == 1) {
						ssd1306_DrawRectangle(45, 52, 80, 62, Black);
						ssd1306_DrawRectangle(69, 15, 92, 32, Black);
						ssd1306_DrawRectangle(32, 15, 57, 31, White); //25x 18y
					}
					if (minAdj == 0) {
						ssd1306_DrawRectangle(45, 52, 80, 62, Black);
						ssd1306_DrawRectangle(32, 15, 57, 32, Black);
						ssd1306_DrawRectangle(69, 15, 92, 31, White);
					}
				}
			}
			if (screen == 1 && chargeSet == 0) {
				if (count == 0) {
					ssd1306_DrawRectangle(0, 10, 120, 22, Black);
					ssd1306_DrawRectangle(0, 23, 120, 35, Black);
					ssd1306_DrawRectangle(0, 52, 50, 62, Black);
					ssd1306_DrawRectangle(0, 36, 120, 48, Black);
					ssd1306_DrawRectangle(65, 52, 120, 62, Black);
				}
				if (count == 1) {
					ssd1306_DrawRectangle(0, 23, 120, 35, Black);
					ssd1306_DrawRectangle(0, 36, 120, 48, Black);
					ssd1306_DrawRectangle(0, 52, 60, 62, Black);
					ssd1306_DrawRectangle(65, 52, 120, 62, Black);
					ssd1306_DrawRectangle(0, 10, 120, 22, White);
				}
				if (count == 2) {
					ssd1306_DrawRectangle(0, 10, 120, 22, Black);
					ssd1306_DrawRectangle(0, 52, 60, 62, Black);
					ssd1306_DrawRectangle(0, 36, 120, 48, Black);
					ssd1306_DrawRectangle(65, 52, 120, 62, Black);
					ssd1306_DrawRectangle(0, 23, 120, 35, White);

				}
				if (count == 3) {
					ssd1306_DrawRectangle(0, 10, 120, 22, Black);
					ssd1306_DrawRectangle(0, 23, 120, 35, Black);
					ssd1306_DrawRectangle(0, 52, 60, 62, Black);
					ssd1306_DrawRectangle(65, 52, 120, 62, Black);
					ssd1306_DrawRectangle(0, 36, 120, 48, White);
				}
				if (count == 4) {
					ssd1306_DrawRectangle(0, 10, 120, 22, Black);
					ssd1306_DrawRectangle(0, 23, 120, 35, Black);
					ssd1306_DrawRectangle(0, 36, 120, 48, Black);
					ssd1306_DrawRectangle(65, 52, 120, 62, Black);
					ssd1306_DrawRectangle(0, 52, 60, 62, White);
				}
				if (count == 5) {
					ssd1306_DrawRectangle(0, 10, 120, 22, Black);
					ssd1306_DrawRectangle(0, 23, 120, 35, Black);
					ssd1306_DrawRectangle(0, 52, 60, 62, Black);
					ssd1306_DrawRectangle(0, 36, 120, 48, Black);
					ssd1306_DrawRectangle(65, 52, 120, 62, White);
				}
			}
			if (screen == 1 && chargeSet == 1) {
				ssd1306_DrawRectangle(55, 36, 90, 48, White);
			}
			if (screen == 6) {
				if (contrastSet == 0 && voltageSetLock == 0 && sleepSet == 0) {
					if (count == 0) {
						ssd1306_DrawRectangle(0, 10, 120, 22, Black);
						ssd1306_DrawRectangle(0, 23, 120, 35, Black);
						ssd1306_DrawRectangle(0, 52, 50, 62, Black);
						ssd1306_DrawRectangle(0, 36, 120, 48, Black);
						ssd1306_DrawRectangle(65, 52, 120, 62, Black);
					}
					if (count == 1) {
						ssd1306_DrawRectangle(0, 23, 120, 35, Black);
						ssd1306_DrawRectangle(0, 36, 120, 48, Black);
						ssd1306_DrawRectangle(0, 52, 60, 62, Black);
						ssd1306_DrawRectangle(65, 52, 120, 62, Black);
						ssd1306_DrawRectangle(0, 10, 120, 22, White);
					}
					if (count == 2) {
						ssd1306_DrawRectangle(0, 10, 120, 22, Black);
						ssd1306_DrawRectangle(0, 52, 60, 62, Black);
						ssd1306_DrawRectangle(0, 36, 120, 48, Black);
						ssd1306_DrawRectangle(65, 52, 120, 62, Black);
						ssd1306_DrawRectangle(0, 23, 120, 35, White);

					}
					if (count == 3) {
						ssd1306_DrawRectangle(0, 10, 120, 22, Black);
						ssd1306_DrawRectangle(0, 23, 120, 35, Black);
						ssd1306_DrawRectangle(0, 52, 60, 62, Black);
						ssd1306_DrawRectangle(65, 52, 120, 62, Black);
						ssd1306_DrawRectangle(0, 36, 120, 48, White);
					}
					if (count == 4) {
						ssd1306_DrawRectangle(0, 10, 120, 22, Black);
						ssd1306_DrawRectangle(0, 23, 120, 35, Black);
						ssd1306_DrawRectangle(0, 52, 60, 62, Black);
						ssd1306_DrawRectangle(0, 36, 120, 48, Black);
						ssd1306_DrawRectangle(65, 52, 120, 62, White);
					}
				}
			}
			if (contrastSet == 1) {
				ssd1306_DrawRectangle(83, 10, 114, 22, White);
			}
			if (voltageSetLock == 1) {
				ssd1306_DrawRectangle(83, 23, 114, 35, White);
			}
			if (sleepSet == 1) {
				ssd1306_DrawRectangle(83, 36, 114, 48, White);
			}
		}

		//////////////////////////// POMIAR NAPIECIA//////////////////////////////////////

		for (int i = 0; i < 2; i++) {
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 1000);
			value[i] = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
		}
		if (milis - voltmilis >= 8) {
			pomiar1 = value[0];
			volt = ((pomiar1 / 4096) * 15.12); //formulae to convert the ADC value to voltage
			voltmilis = milis;
		}
		/////////////konwersja liczby na char //////////////////////////////
		snprintf(voltage, 5, "%f", volt);
		printf("%s", voltage);

		snprintf(voltageSet, 5, "%f", voltSet);
		printf("%s", voltageSet);

		snprintf(contrastLcd, 4, "%d", contrast);
		printf("%s", contrastLcd);

		snprintf(sleepLcd, 4, "%d", sleep);
		printf("%s", sleepLcd);

		snprintf(timeSek, 7, "%d", sek);
		printf("%s", timeSek);
		snprintf(timeMin, 7, "%d", min);
		printf("%s", timeMin);

		snprintf(timeSekGth, 7, "%d", sekGth);
		printf("%s", timeSekGth);
		snprintf(timeMinGth, 7, "%d", minGth);
		printf("%s", timeMinGth);

		snprintf(timeSetSek, 7, "%d", sekSet);
		printf("%s", timeSetSek);
		snprintf(timeSetMin, 7, "%d", minSet);
		printf("%s", timeSetMin);

		snprintf(timeSetSekGth, 7, "%d", sekSetGth);
		printf("%s", timeSetSekGth);
		snprintf(timeSetMinGth, 7, "%d", minSetGth);
		printf("%s", timeSetMinGth);
		/////////////////////////////////Ekran//////////////////////////////
		dots();
		switch (screen) {
		case 0:   //////////////// ekran główny
			//			ssd1306_DrawRectangle(0, 12, 120, 35, White);
			ssd1306_SetCursor(30, 0);
			ssd1306_WriteString("TURBOTIMER", Font_7x10, White);
			ssd1306_SetCursor(35, 33);
			ssd1306_WriteString("min", Font_7x10, White);
			ssd1306_SetCursor(70, 33);
			ssd1306_WriteString("sek", Font_7x10, White);
			ssd1306_SetCursor(58, 16);
			ssd1306_WriteString(":", Font_11x18, White);
			if (sek >= 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString(timeSek, Font_11x18, White);
			}
			if (sek < 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(80, 16);
				ssd1306_WriteString(timeSek, Font_11x18, White);
			}
			if (min >= 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString(timeMin, Font_11x18, White);
			}
			if (min < 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(45, 16);
				ssd1306_WriteString(timeMin, Font_11x18, White);
			}
			ssd1306_SetCursor(10, 50);
			ssd1306_WriteString("Voltage :", Font_7x10, White);
			ssd1306_SetCursor(74, 50);
			ssd1306_WriteString(voltage, Font_7x10, White);
			ssd1306_SetCursor(105, 50);
			ssd1306_WriteString("V", Font_7x10, White);
			break;
		case 1:
			ssd1306_SetCursor(50, 0);
			ssd1306_WriteString("MENU", Font_6x8, White);
			ssd1306_SetCursor(3, 13);
			ssd1306_WriteString("TURBOTIMER", Font_6x8, White);
			ssd1306_SetCursor(3, 26);
			ssd1306_WriteString("GO TO HOME", Font_6x8, White);
			ssd1306_SetCursor(3, 39);
			ssd1306_WriteString("CHARGE :", Font_6x8, White);
			ssd1306_SetCursor(13, 54);
			ssd1306_WriteString("CONFIG", Font_6x8, White);
			ssd1306_SetCursor(87, 54);
			ssd1306_WriteString("EXIT", Font_6x8, White);
			if (charge == 0) {
				ssd1306_SetCursor(60, 39);
				ssd1306_WriteString("Off ", Font_6x8, White);
			}
			if (charge == 1) {
				ssd1306_SetCursor(60, 39);
				ssd1306_WriteString(" On", Font_6x8, White);
			}

			break;
		case 2:
			ssd1306_SetCursor(55, 54);
			ssd1306_WriteString("OK", Font_6x8, White);
			ssd1306_SetCursor(15, 0);
			ssd1306_WriteString("TURBOTIMER SET", Font_7x10, White);
			ssd1306_SetCursor(35, 33);
			ssd1306_WriteString("min", Font_7x10, White);
			ssd1306_SetCursor(70, 33);
			ssd1306_WriteString("sek", Font_7x10, White);
			ssd1306_SetCursor(58, 16);
			ssd1306_WriteString(":", Font_11x18, White);
			if (sekSet >= 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString(timeSetSek, Font_11x18, White);
			}
			if (sekSet < 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(80, 16);
				ssd1306_WriteString(timeSetSek, Font_11x18, White);
			}
			if (minSet >= 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString(timeSetMin, Font_11x18, White);
			}
			if (minSet < 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(45, 16);
				ssd1306_WriteString(timeSetMin, Font_11x18, White);
			}
			if (minAdj == 0) {
				if (count - prevCount <= -1) {
					sekSet--;
					prevCount = count;
				}
				if (sekSet < 0) {
					sekSet = 59;
				}
				if (count - prevCount >= 1) {
					sekSet++;
					prevCount = count;
				}
				if (sekSet > 59) {
					sekSet = 0;
				}
			}
			if (minAdj == 1) {
				if (count - prevCount <= -1) {
					minSet--;
					prevCount = count;
				}
				if (minSet < 0) {
					minSet = 20;
				}
				if (count - prevCount >= 1) {
					minSet++;
					prevCount = count;
				}
				if (minSet > 20) {
					minSet = 0;
				}
			}
			break;
		case 3:
			ssd1306_SetCursor(55, 54);
			ssd1306_WriteString("OK", Font_6x8, White);
			ssd1306_SetCursor(15, 0);
			ssd1306_WriteString("GO TO HOME SET", Font_7x10, White);
			ssd1306_SetCursor(35, 33);
			ssd1306_WriteString("min", Font_7x10, White);
			ssd1306_SetCursor(70, 33);
			ssd1306_WriteString("sek", Font_7x10, White);
			ssd1306_SetCursor(58, 16);
			ssd1306_WriteString(":", Font_11x18, White);
			if (sekSetGth >= 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString(timeSetSekGth, Font_11x18, White);
			}
			if (sekSetGth < 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(80, 16);
				ssd1306_WriteString(timeSetSekGth, Font_11x18, White);
			}
			if (minSetGth >= 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString(timeSetMinGth, Font_11x18, White);
			}
			if (minSetGth < 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(45, 16);
				ssd1306_WriteString(timeSetMinGth, Font_11x18, White);
			}
			if (minAdj == 0) {
				if (count - prevCount <= -1) {
					sekSetGth--;
					prevCount = count;
				}
				if (sekSetGth < 0) {
					sekSetGth = 59;
				}
				if (count - prevCount >= 1) {
					sekSetGth++;
					prevCount = count;
				}
				if (sekSetGth > 59) {
					sekSetGth = 0;
				}
			}
			if (minAdj == 1) {
				if (count - prevCount <= -1) {
					minSetGth--;
					prevCount = count;
				}
				if (minSetGth < 0) {
					minSetGth = 5;
				}
				if (count - prevCount >= 1) {
					minSetGth++;
					prevCount = count;
				}
				if (minSetGth > 5) {
					minSetGth = 0;
				}
			}
			break;
		case 4:
			ssd1306_SetCursor(13, 19);
			ssd1306_WriteString("CHARGE ON", Font_11x18, White);
			ssd1306_SetCursor(10, 50);
			ssd1306_WriteString("Voltage :", Font_7x10, White);
			ssd1306_SetCursor(74, 50);
			ssd1306_WriteString(voltage, Font_7x10, White);
			ssd1306_SetCursor(105, 50);
			ssd1306_WriteString("V", Font_7x10, White);
			break;
		case 5:
			ssd1306_SetCursor(18, 19);
			ssd1306_WriteString("STARTING", Font_11x18, White);
			break;
		case 6:
			ssd1306_SetCursor(15, 0);
			ssd1306_WriteString("CONFIGURATION", Font_6x8, White);
			ssd1306_SetCursor(3, 13);
			ssd1306_WriteString("CONTRAST :", Font_6x8, White);
			ssd1306_SetCursor(94, 13);
			ssd1306_WriteString(contrastLcd, Font_6x8, White);
			ssd1306_SetCursor(3, 26);
			ssd1306_WriteString("VOLTAGE SET :", Font_6x8, White);
			ssd1306_SetCursor(87, 26);
			ssd1306_WriteString(voltageSet, Font_6x8, White);
			ssd1306_SetCursor(3, 39);
			ssd1306_WriteString("SLEEP TIME :", Font_6x8, White);
			ssd1306_SetCursor(94, 39);
			ssd1306_WriteString(sleepLcd, Font_6x8, White);
			//			ssd1306_SetCursor(13, 54);
			//			ssd1306_WriteString("CONFIG", Font_6x8, White);
			ssd1306_SetCursor(87, 54);
			ssd1306_WriteString("EXIT", Font_6x8, White);
			break;
		case 7:
			ssd1306_SetCursor(30, 0);
			ssd1306_WriteString("GO TO HOME", Font_7x10, White);
			ssd1306_SetCursor(35, 33);
			ssd1306_WriteString("min", Font_7x10, White);
			ssd1306_SetCursor(70, 33);
			ssd1306_WriteString("sek", Font_7x10, White);
			ssd1306_SetCursor(58, 16);
			ssd1306_WriteString(":", Font_11x18, White);
			if (sekGth >= 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString(timeSekGth, Font_11x18, White);
			}
			if (sekGth < 10) {
				ssd1306_SetCursor(70, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(80, 16);
				ssd1306_WriteString(timeSekGth, Font_11x18, White);
			}
			if (minGth >= 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString(timeMinGth, Font_11x18, White);
			}
			if (minGth < 10) {
				ssd1306_SetCursor(35, 16);
				ssd1306_WriteString("0", Font_11x18, White);
				ssd1306_SetCursor(45, 16);
				ssd1306_WriteString(timeMinGth, Font_11x18, White);
			}
			ssd1306_SetCursor(10, 50);
			ssd1306_WriteString("Voltage :", Font_7x10, White);
			ssd1306_SetCursor(74, 50);
			ssd1306_WriteString(voltage, Font_7x10, White);
			ssd1306_SetCursor(105, 50);
			ssd1306_WriteString("V", Font_7x10, White);
			break;
		case 8:
			//			ssd1306_SetCursor(10, 0);
			//			ssd1306_WriteString("DJD CUSTOM", Font_11x18, White);
			//			ssd1306_SetCursor(8, 25);
			//			ssd1306_WriteString("Software ver 3.1", Font_7x10, White);
			//			ssd1306_SetCursor(10, 50);
			//			ssd1306_WriteString("INITIALIZATION", Font_7x10, White);
			ssd1306_startLogo();
			//			ssd1306_TestDrawBitmap();
			break;

		}
		ssd1306_UpdateScreen();
		//		HAL_Delay(1);
		/////////////////////////////////////////////////////////////////////////////////////

		///////////////////////// PRZYCISKI ////////////////////////////////////////////////
		enter = HAL_GPIO_ReadPin(GPIOA, OK_Pin);
		reczny = HAL_GPIO_ReadPin(GPIOA, B1_Pin);
		kierunkowskaz = HAL_GPIO_ReadPin(GPIOB, Blink_Pin);
		//////////////////////Przycisk sport//////////////////////////////////////////////
		sportState = HAL_GPIO_ReadPin(GPIOA, Sport_Pin);

				if (sportState == 1 && sportSet == 0) {
					if (milis - prevmilis3 < 5000) {
						HAL_GPIO_WritePin(GPIOB, open_Pin, GPIO_PIN_SET);
					}
					if (milis - prevmilis3 > 5000) {
						HAL_GPIO_WritePin(GPIOB, open_Pin, GPIO_PIN_RESET);
						prevmilis3 = milis;
						sportSet = 1;
					}
				}
				if (sportState == 0 && sportSet == 1) {
					if (milis - prevmilis3 < 5000) {
						HAL_GPIO_WritePin(GPIOB, close_Pin, GPIO_PIN_SET);
					}
					if (milis - prevmilis3 > 5000) {
						HAL_GPIO_WritePin(GPIOB, close_Pin, GPIO_PIN_RESET);
						prevmilis3 = milis;
						sportSet = 0;
					}
				}
///////////////////////////////////////////////////////////////////////////////////////////////

		if (enter == 0 && lock == 0) {
			pressed = 1;
			lock = 1;
		}
		if (enter == 1 && lock == 1) {
			lock = 0;
		}
		if (pressed == 1 && lock == 0 && display == 1) {
			ssd1306_Fill(Black);
			if (screen == 2) {
				if (minAdj == 2) {
					sek = sekSet;
					min = minSet;
					screen = 1;
					count = 0;
				}
				if (minAdj == 1) {
					minAdj = 2;
				}
				if (minAdj == 0) {
					minAdj = 1;
				}
			}
			if (screen == 3) {
				if (minAdj == 2) {
					sekGth = sekSetGth;
					minGth = minSetGth;
					screen = 1;
				}
				if (minAdj == 1) {
					minAdj = 2;
				}
				if (minAdj == 0) {
					minAdj = 1;
				}
			}
			if (screen == 0 && turbotimerOn == 0) {
				//				count = 0;
				screen = 1;
			}
			if (screen == 4) {
				//				count = 0;
				screen = 1;
			}
			if (screen == 1) {
				if (count == 0) {
					minAdj = 0;
				}
				if (count == 1 && chargeSet == 0) {
					sekSet = sek;
					minSet = min;
					count = 0;
					prevCount = 0;
					prevCounter = 32767;
					TIM3->CNT = 32767;
					counter = 32767;
					screen = 2;
				}
				if (count == 2) {
					sekSetGth = sekGth;
					minSetGth = minGth;
					count = 0;
					prevCount = 0;
					prevCounter = 32767;
					TIM3->CNT = 32767;
					counter = 32767;
					screen = 3;
				}
				if (count == 3 && chargeSet == 0 && setLock == 0) {
					count = charge;
					prevCount = charge;
					prevCounter = 32767;
					TIM3->CNT = 32767;
					counter = 32767;
					chargeSet = 1;
					setLock = 1;
				}
				if (chargeSet == 1 && setLock == 0) {
					charge = count;
					chargeSet = 0;
					setLock = 1;
				}
				if (count == 4) {
					count = 0;
					counter = 0;
					prevCounter = 0;
					prevCount = 0;
					TIM3->CNT = 0;
					screen = 6;
				}
				if (count == 5) {
					if (charge == 1) {
						screen = 4;
					}
					if (charge == 0) {
						screen = 0;
					}
				}
			}
			if (screen == 6) {
				if (count == 1 && contrastSet == 0 && setLock == 0) {
					count = contrast;
					prevCount = contrast;
					prevCounter = 32767;
					TIM3->CNT = 32767;
					counter = 32767;
					contrastSet = 1;
					setLock = 1;
				}
				if (contrastSet == 1 && setLock == 0) {
					count = 0;
					counter = 0;
					prevCounter = 0;
					prevCount = 0;
					TIM3->CNT = 0;
					contrastSet = 0;
					setLock = 1;
				}
				if (count == 2 && voltageSetLock == 0 && setLock == 0) {
					count = (voltSet * 10.0);
					prevCount = (voltSet * 10);
					prevCounter = 32767;
					TIM3->CNT = 32767;
					counter = 32767;
					voltageSetLock = 1;
					setLock = 1;

				}
				if (voltageSetLock == 1 && setLock == 0) {
					count = 0;
					counter = 0;
					prevCounter = 0;
					prevCount = 0;
					TIM3->CNT = 0;
					voltageSetLock = 0;
					setLock = 1;
				}
				if (count == 3 && sleepSet == 0 && setLock == 0) {
					count = sleep;
					prevCount = sleep;
					prevCounter = 32767;
					TIM3->CNT = 32767;
					counter = 32767;
					sleepSet = 1;
					setLock = 1;
				}
				if (sleepSet == 1 && setLock == 0) {
					count = 0;
					counter = 0;
					prevCounter = 0;
					prevCount = 0;
					TIM3->CNT = 0;
					sleepSet = 0;
					setLock = 1;
				}
				if (count == 4) {
					screen = 1;
				}
			}
			if (screen == 1 || screen == 0 || screen == 4 || screen == 8) {
				if (count != 3 && chargeSet != 1) {
					count = 0;
					counter = 0;
					prevCounter = 0;
					prevCount = 0;
					TIM3->CNT = 0;
				}
			}
			if (volt < voltSet && gthLock == 0 && screen == 7) {
				minGth = 0;
				sekGth = 1;
			}
			if (volt > voltSet && turbotimerOn == 1 && screen == 0) {
				min = 7;
				sek = 0;
			}
			//			if (screen == 6) {
			//				if (count != 1 && contrastSet != 1) {
			//					count = 0;
			//					counter = 0;
			//					prevCounter = 0;
			//					prevCount = 0;
			//					TIM3->CNT = 0;
			//				}
			//			}
			turbotimerLock = 0;
			setLock = 0;
			pressed = 0;
		}
		if (pressed == 1 && lock == 0 && display == 0) {
			screen = 8;
			ssd1306_SetDisplayOn(1);
			if (screen == 8) {
				if (milis - prevmilis2 >= 50) {
					ssd1306_Fill(Black);
					screen = 0;
					prevmilis2 = milis;
				}
			}
			//			ssd1306_SetDisplayOn(1);
			turbotimerLock = 0;
			setLock = 0;
			pressed = 0;
		}

		///////////////////////////////////////////////////////////////////////////////////
		////////////////////////Działanie softu/////////////////////////////////////////
		display = ssd1306_GetDisplayOn();
		if (screen == 8) {
			prevmilisBckl = milis;
			if (milis - prevmilis2 >= 50) {
				ssd1306_Fill(Black);
				screen = 0;
				prevmilis2 = milis;
			}
			//			HAL_Delay(5000);
			//			ssd1306_Fill(Black);
			//			screen = 0;
		}
		if (screen != 8) {
			prevmilis2 = milis;
		}
		if (display == 0 && volt < voltSet) {
			ssd1306_Fill(Black);
			prevmilisBckl = milis;
			screen = 8;
		}
		if (screen == 0 && volt >= voltSet) {
			ssd1306_Fill(Black);
			if (reczny == 0 && turbotimerOn == 0) {
				//				ssd1306_SetDisplayOn(1);
				HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_SET);
				//				turbotimerLock = 0;
				screen = 5;
			}
			if (reczny == 1 && turbotimerLock == 1) {
				HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_SET);
				turbotimerLock = 0;
			}
			if (reczny == 1 && turbotimerOn == 0) {
				HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_SET);
				turbotimerLock = 0;
			}
			if (reczny == 0 && turbotimerLock == 0) {
				HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_RESET);
//				screen = 0;
			}
		}
		if (screen == 5 && volt >= voltSet) {
			if (reczny == 0) {
				ssd1306_SetDisplayOn(1);
				if (trigger < 10) {
					//					screen = 5;
					trigger++;
				}
				if (trigger == 10) {
					screen = 0;
					turbotimerOn = 1;
				}
			}
			if (reczny == 1) {
				HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_SET);
				screen = 0;
			}
		}
		if (reczny == 1) {
//			HAL_GPIO_WritePin(GPIOB, ttRelay_Pin, GPIO_PIN_RESET);
			turbotimerOn = 0;
			trigger = 0;
			sek = sekSet;
			min = minSet;
		}
		if (volt > voltSet && charge == 0) {
			HAL_GPIO_WritePin(GPIOB, GthRelay_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GthLamp1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GthLamp2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, GthLicznik_Pin, GPIO_PIN_RESET);
			gthLock = 0;
			sekGth = sekSetGth;
			minGth = minSetGth;
			carLocked = 0;
			carUnLocked = 0;
		}
		if (volt > voltSet && charge == 1) {
			gthLock = 1;
			sekGth = 0;
			minGth = 0;
		}
		if (volt < 11.0) {
			gthLock = 1;
			HAL_GPIO_WritePin(GPIOB, GthRelay_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLamp1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLamp2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GthLicznik_Pin, GPIO_PIN_SET);
		}
		if (volt < voltSet && voltageSetLock == 0) {
			if (turbotimerOn == 1) {
				ssd1306_SetDisplayOn(1);
			}
			if (gthLock == 0 && screen != 8) {
				screen = 7;
			}
			if (screen == 0 && gthLock == 1) {
				if (milis - prevmilisBckl >= (sleep * 10)) {
					if (charge == 0 && turbotimerOn == 0) {
						ssd1306_SetDisplayOn(0);
					}
					prevmilisBckl = milis;
				}
			}
			if (screen == 7 && gthLock == 1) {
				if (milis - prevmilisBckl >= (sleep * 10)) {
					if (charge == 0 && turbotimerOn == 0) {
						ssd1306_SetDisplayOn(0);
					}
					if (charge == 0 && turbotimerOn == 1) {
						ssd1306_SetDisplayOn(0);
					}
					prevmilisBckl = milis;
				}
			}

			////////////////////////////Obsługa kierunkowskazów//////////////////////////////////////
			if (blinkCount == 1 && display == 1) {
				gthLock = 0;
				sekGth = sekSetGth;
				minGth = minSetGth;
				carLocked = 1;
				carUnLocked = 0;
			}
			if (blinkCount == 2) {
				screen = 7;
				ssd1306_SetDisplayOn(1);
				gthLock = 0;
				sekGth = sekSetGth;
				minGth = minSetGth;
				carLocked = 0;
				carUnLocked = 1;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////
		if (volt > voltSet && voltageSetLock == 0 && charge == 0) {
			if (display == 0) {
				ssd1306_Fill(Black);
				screen = 8;
				ssd1306_SetDisplayOn(1);
				prevmilisBckl = milis;
			}
			if (screen == 7) {
				screen = 0;
			}
		}
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief IWDG Initialization Function
  * @param None
  * @retval None
  */
static void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 15;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 15;
  if (HAL_TIM_Encoder_Init(&htim3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, close_Pin|open_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GthLamp2_Pin|ttRelay_Pin|GthRelay_Pin|GthLicznik_Pin
                          |GthLamp1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : B1_Pin OK_Pin */
  GPIO_InitStruct.Pin = B1_Pin|OK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Blink_Pin Sport_Pin */
  GPIO_InitStruct.Pin = Blink_Pin|Sport_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : close_Pin open_Pin */
  GPIO_InitStruct.Pin = close_Pin|open_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GthLamp2_Pin ttRelay_Pin GthRelay_Pin GthLicznik_Pin
                           GthLamp1_Pin */
  GPIO_InitStruct.Pin = GthLamp2_Pin|ttRelay_Pin|GthRelay_Pin|GthLicznik_Pin
                          |GthLamp1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
