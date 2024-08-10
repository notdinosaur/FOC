/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "k210.h"
#include "foc.h"
#include "svpwm.h"
#include "AS5600.h"
#include "math.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
struct __FILE  //printf重映射
{
  int handle;
};
FILE __stdout;
int fputc(int ch, FILE *f) 
{
  HAL_UART_Transmit(&huart1,(unsigned char *)&ch ,1,50);
  return ch;
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
FOC motor_1;
FOC motor_2;
SV sv_1;
SV sv_2;
Transfer tran_1;
Transfer tran_2;
float moto_1=0.4,moto_2=4;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t USART2_RXbuff =0;
uint8_t USART3_RXbuff =0;
extern int xx;                 //摄像头发送的数据
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2,&USART2_RXbuff,1);
	HAL_UART_Receive_IT(&huart3,(uint8_t*)&USART3_RXbuff,1);

  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);

  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim4);  
  /* USER CODE END 2 */
  foc_init(&motor_1);
  foc_init(&motor_2);
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    //printf("111");
  	//编码器获取电角度
		motor_1.Theat_mac = i2c_AS5600_get_angle();
		GetElectricAngle(&motor_1);
    /* USER CODE BEGIN 3 */
		if(USART3_RXbuff==97)
		{
			moto_1 = 0.4;
			moto_2 = 4;
		}
		if(USART3_RXbuff==98)
		{
			moto_1 = -0.4;
			moto_2 = -4;			
		}
		if(USART3_RXbuff<=10&&USART3_RXbuff>0)
		{
			moto_1 = 0.1f*USART3_RXbuff;
			moto_2 = 1.0f*USART3_RXbuff;
		}
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim3))
    {
      //printf("111");
      Inverse_Park_Transfer(&tran_1,0,moto_1,sin(motor_1.Theat_ele),cos(motor_1.Theat_ele));
      SvpwmControl(&sv_1,tran_1.Ualpha,tran_1.Ubeta,1.0f,8400,16);

		  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,sv_1.ccr1);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,sv_1.ccr2);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,sv_1.ccr3);


      Inverse_Park_Transfer(&tran_2,0,moto_2,sin(-1*motor_1.Theat_ele),cos(-1*motor_1.Theat_ele));
      SvpwmControl(&sv_2,tran_2.Ualpha,tran_2.Ubeta,1.0f,8400,16);

		  __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,sv_2.ccr1);
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_2,sv_2.ccr2);
			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,sv_2.ccr3);
			
    }		
		

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)    //摄像头数据返回
	{
    
		k210_Receive(USART2_RXbuff);
		HAL_UART_Receive_IT(&huart2,&USART2_RXbuff,1);
		
	}
	if(huart->Instance == USART3)    //屏幕数据返回 
	{
		 printf("%d\r\n",USART3_RXbuff);
		HAL_UART_Receive_IT(&huart3,(uint8_t*)&USART3_RXbuff,1);
		
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
