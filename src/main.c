#include "main.h"
#include "usart.h"
#include "usbd_audio.h"
#include "st7735.h"
#include "st7735_misc.h"
#include <arm_math.h>
USBD_HandleTypeDef USBD_Device;
AUDIO_STATUS_TypeDef audio_status;
extern bool firstHalf;
void SystemClock_Config(void);

int main(void) {
  
  extern uint32_t LogWindowOpenedTimeStart;
  extern bool flLogWindowOpened;
  extern bool flgBufferCopyComplete;
  extern bool flgBufferCopyCompleteFull;
  extern void FFT_Init_FFTHandlers(void);
  extern void FFT_TEST_Array(uint32_t audiofreq);

  
  static const uint8_t STEPS_COUNT = 7U;
  static const char* P_STEPS[] = {
                            ":..",
                            ".P.",
                            ".P:",
                            ":P:",
                            ":P:",
                            ".::",
                            "..:",
                            };

  HAL_Init();
  SystemClock_Config();

  MX_USART2_UART_Init();


  bsp_init();
  FFT_Init_FFTHandlers();

  printMsg("USB I2S Audio Bridge");

  // Init Device Library
  USBD_Init(&USBD_Device, &AUDIO_Desc, 0);
  // Add Supported Class
  USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);
  // Add Interface callbacks for AUDIO Class
  USBD_AUDIO_RegisterInterface(&USBD_Device, &USBD_AUDIO_fops);
  // Start Device Process
  USBD_Start(&USBD_Device);
  
  while (1) {
    switch (audio_status.frequency) {
      case 44100:
          BSP_LED_Off(LED_RED);
          BSP_LED_Off(LED_GREEN);
          BSP_LED_On(LED_BLUE);
          break;
      case 48000:
          BSP_LED_Off(LED_RED);
          BSP_LED_On(LED_GREEN);
          BSP_LED_Off(LED_BLUE);
          break;
      case 96000:
          BSP_LED_On(LED_RED);
          BSP_LED_Off(LED_GREEN);
          BSP_LED_Off(LED_BLUE);
          break;
      default:
          BSP_LED_Off(LED_RED);
          BSP_LED_Off(LED_GREEN);
          BSP_LED_Off(LED_BLUE);
          break;
    }

    if(audio_status.playing)
    {
      static uint16_t current_step = 0;

      BSP_LED_Playing(P_STEPS[current_step++]);
      if(current_step == STEPS_COUNT){
        current_step = 0;
        // HAL_Delay();
      }
    }

    if(flLogWindowOpened){
      uint32_t CurrentTicks = HAL_GetTick();
      if( (CurrentTicks - LogWindowOpenedTimeStart) >= (LOG_WINDOW_TIMEOUT)){
        CLOSE_LOG_WINDOW();
      }
    }

    #ifdef DEBUG_FEEDBACK_ENDPOINT
    {
        extern uint8_t fftFlag;
        if(fftFlag == 1U){
            UNUSED(fftFlag);
        }
    }    
    #endif
    {

      if(flgBufferCopyComplete){  
        firstHalf = true;
        FFT_TEST_Array(audio_status.frequency);
        flgBufferCopyComplete = false;
      }

      if(flgBufferCopyCompleteFull){
        firstHalf = false;
        FFT_TEST_Array(audio_status.frequency);
        flgBufferCopyCompleteFull = false;
      }

      extern float peakValL; extern float peakValR;
      extern void DrawPeaksIndicator(float LVal, float RVal);
      DrawPeaksIndicator(peakValL,peakValR);
      peakValL = (sqrtf(peakValL*peakValR))/4;
      peakValR = (sqrtf(peakValL*peakValR))/4;

    }

#ifdef DEBUG_FEEDBACK_ENDPOINT // see Makefile C_DEFS
    // see USBD_AUDIO_SOF() in usbd_audio.c
	if (BtnPressed) {
		BtnPressed = 0;
		printMsg("DbgSfZ_WrSmpls=%d", AUDIO_BUF_SAFEZONE_SAMPLES);
    printMsg("DbgOpt_WrSmpls=%d", AUDIO_TOTAL_BUF_SIZE/(2*6));
		printMsg("DbgMax_WrSmpls=%d", DbgMaxWritableSamples);
    printMsg("DbgMin_WrSmpls=%d", DbgMinWritableSamples);
		int count = 256;
		while (count--){
			// print oldest to newest
			printMsg("%d %d %f", DbgSofHistory[DbgIndex], DbgWritableSampleHistory[DbgIndex], DbgFeedbackHistory[DbgIndex]);
			DbgIndex++;
			}
		}
#endif

  }
}


// STM32F411CEU6 versus STM32F401CCU6 "Black Pill" 

#ifdef STM32F411xE

// 96MHz system clock (max 100MHz)
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 271;
  PeriphClkInitStruct.PLLI2S.PLLI2SM = 25;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif

#ifdef STM32F401xC
// 84MHz system clock (max = 84MHz)
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 271;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif

void printMsg(char* format, ...) {
	char sz[100];
	va_list args;
	va_start(args, format);
	vsprintf(sz, format, args);
	HAL_UART_Transmit(&huart2, (uint8_t *)sz, strlen(sz), HAL_MAX_DELAY);
    
    DRAW_LOG_MESSAGE(sz,LOG_MSG_NRM_CLR,current_log_y);
    // HAL_Delay();
	va_end(args);
	}

void Error_Handler(void){
	uint32_t counter;
	while(1){
		BSP_OnboardLED_Toggle();
		counter = 0xFFFF;
		while (counter--) ;
	}

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
    printMsg("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */

