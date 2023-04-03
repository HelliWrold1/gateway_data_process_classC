#ifndef __TREMO_IT_H
#define __TREMO_IT_H

#ifdef __cplusplus
extern "C" {
#endif

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
extern unsigned char UART_DATA[25];
extern unsigned char UART_DATA_LENGTH;
extern unsigned char UART_RX_DONE;

#ifdef __cplusplus
}
#endif

#endif /* __TREMO_IT_H */
