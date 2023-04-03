#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "tremo_uart.h"
#include "tremo_gpio.h"
#include "tremo_rcc.h"

void uart_log_init(void)
{
    // uart0
    gpio_set_iomux(GPIOB, GPIO_PIN_0, 1);
    gpio_set_iomux(GPIOB, GPIO_PIN_1, 1);

    /* uart config struct init */
    uart_config_t uart_config;
    uart_config_init(&uart_config);

    uart_config.baudrate = UART_BAUDRATE_115200;
    uart_init(UART0, &uart_config);
    uart_cmd(UART0, ENABLE);
}

int main(void)
{
	char a[256];
    /* Add your application code here */
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
	uart_log_init();
    /* Infinite loop */
	printf("hello world\r\n");	
    while (1) { 
		if(uart_receive_data(UART0)){
			strcat(a,(char *)uart_receive_data(UART0));
		}else{
			printf("%s",a);
			continue;
		}			
	}
	
}

#ifdef USE_FULL_ASSERT
void assert_failed(void* file, uint32_t line)
{
    (void)file;
    (void)line;

    while (1) { }
}
#endif
