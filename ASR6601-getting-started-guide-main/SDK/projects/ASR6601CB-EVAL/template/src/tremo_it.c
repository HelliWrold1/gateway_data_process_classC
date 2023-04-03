
#include "tremo_it.h"
#include "tremo_regs.h"
#include "tremo_uart.h"
#include "stdio.h"
#include "string.h"

extern void RadioOnDioIrq(void);
extern void RtcOnIrq(void);
unsigned char UART_DATA[25] = {0};
unsigned char UART_DATA_LENGTH = 0;
unsigned char UART_RX_DONE = 0;
u8 cur = 0;

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{

    /* Go to infinite loop when Hard Fault exception occurs */
    while(1){
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while(1){
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while(1){
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while(1){
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles PWR Handler.
  * @param  None
  * @retval None
  */
void PWR_IRQHandler()
{
}

/******************************************************************************/
/*                 Tremo Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_cm4.S).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void UART0_IRQHandler(void)
{
	u8 Res;
	if (UART_INTERRUPT_RX_DONE == (1 << 4)){ // if rx is done
		Res = uart_receive_data(UART0);
		if(Res!=0x0a){ // if not 0x0a
			UART_DATA[cur] = Res; // get the uart data
			cur += 1; // point to next
			// if the index 24 is not 0x0a, then the UART_DATA is invaild data
			if (cur > 23){ // reset point
				cur = 0; 
				memset(UART_DATA,0,25); // clear the UART_DATA
				UART_RX_DONE = 0;
				UART_DATA_LENGTH = 0;
			}else{
				printf("%c",Res);
			}
		}else{
			UART_DATA[cur - 1] = 0; // remove the 0x0d
			UART_DATA_LENGTH = cur - 1; // datalength:from 1 to last
			UART_RX_DONE = 1;
			cur = 0; // reset point
		}
	}	
}

void LORA_IRQHandler()
{
    RadioOnDioIrq();
}

void RTC_IRQHandler(void)
{
    RtcOnIrq();
}