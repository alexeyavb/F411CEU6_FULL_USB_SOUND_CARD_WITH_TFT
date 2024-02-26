#include "main.h"
#include "bsp_misc.h"
#include "tim.h"
#include "spi.h"
#include "st7735.h"
#include "st7735_misc.h"
void Error_Handler(void);
void BSP_Peryph_Init(void);

volatile uint32_t BtnPressed = 0;

void bsp_init(void) {
	BSP_PB_Init();
	BSP_Peryph_Init();
	BSP_LED_Init();
	}

void BSP_Peryph_Init(void){
	MX_SPI5_Init();
	ST7735_Init();
	MX_TIM4_Init();
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	SetPWM(99);
	ST7735_FillScreen(ST7735_BLACK);
	SetPWM(30);  
	InitTop();

	DRAW_LOG_WINDOW();
	UNUSED(current_log_y);
}

#ifdef BOARD_USE_EXTERNAL_HARDWARE_LED
uint32_t GPIO_PIN[3] = {
	LED_RED_PIN,
    LED_GREEN_PIN,
    LED_BLUE_PIN
};

void BSP_LED_Init(void) {
	GPIO_InitTypeDef  gpio_init_structure = {0};
	gpio_init_structure.Pin   = LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN;
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_LOW;

	LED_GPIO_CLK_ENABLE();
	HAL_GPIO_Init(LED_GPIO_PORT, &gpio_init_structure);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_SET); // R,G,B LEDs are active low
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_BLUE_PIN, GPIO_PIN_SET);

	gpio_init_structure.Pin   = ONBOARD_LED_PIN;
	ONBOARD_LED_GPIO_CLK_ENABLE();
	HAL_GPIO_Init(ONBOARD_LED_PORT, &gpio_init_structure);
	HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_SET); // onboard led is active low
}
void BSP_LED_DeInit(void){
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_RED_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);

	GPIO_InitTypeDef  gpio_init_structure = {0};
	gpio_init_structure.Pin   = LED_RED_PIN | LED_GREEN_PIN | LED_BLUE_PIN;
	HAL_GPIO_DeInit(LED_GPIO_PORT, gpio_init_structure.Pin);

	gpio_init_structure.Pin   = ONBOARD_LED_PIN;
	HAL_GPIO_DeInit(ONBOARD_LED_PORT, gpio_init_structure.Pin);
}

void BSP_LED_On(Led_TypeDef Led) {
  if (Led < 3)  {
     HAL_GPIO_WritePin(LED_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_RESET);
  }
}


void BSP_LED_Off(Led_TypeDef Led){
  if (Led < 3)  {
    HAL_GPIO_WritePin(LED_GPIO_PORT, GPIO_PIN[Led], GPIO_PIN_SET);
  }
}


void BSP_LED_Toggle(Led_TypeDef Led){
  if (Led < 3)  {
     HAL_GPIO_TogglePin(LED_GPIO_PORT, GPIO_PIN[Led]);
  }
}

#else
void BSP_LED_Init(void) {
	GPIO_InitTypeDef  gpio_init_structure = {0};
	gpio_init_structure.Pin   = ONBOARD_LED_PIN;
	gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
	gpio_init_structure.Pull  = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_LOW;
	ONBOARD_LED_GPIO_CLK_ENABLE();
	HAL_GPIO_Init(ONBOARD_LED_PORT, &gpio_init_structure);
	HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_SET); // onboard led is active low
}

void BSP_LED_DeInit(void){
	GPIO_InitTypeDef  gpio_init_structure = {0};
	HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);	
	gpio_init_structure.Pin   = ONBOARD_LED_PIN;
	HAL_GPIO_DeInit(ONBOARD_LED_PORT, gpio_init_structure.Pin);
}

__weak void BSP_LED_On(Led_TypeDef Led) {
	UNUSED(Led);
}
__weak void BSP_LED_Off(Led_TypeDef Led) {
	UNUSED(Led);
}
__weak void BSP_LED_Toggle(Led_TypeDef Led) {
	UNUSED(Led);
}
#endif


// Onboard blue led C13
void BSP_OnboardLED_On(void) {
    HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_RESET);
}

void BSP_OnboardLED_Off(void) {
    HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, GPIO_PIN_SET);
}

void BSP_OnboardLED_Toggle(void) {
    HAL_GPIO_TogglePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN);
}


void BSP_PB_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /*Configure GPIO pin : PA0  (KEY button on board) */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

// Onboard user button
uint32_t BSP_PB_GetState(void) {
  return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t gpioPin) {
	if (gpioPin == GPIO_PIN_0) {
		BtnPressed = 1;
	}
}

