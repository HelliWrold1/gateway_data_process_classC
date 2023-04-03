#include <stdio.h>
#include "tremo_rcc.h"
#include "tremo_gpio.h"
#include "tremo_i2c.h"
#include "tremo_delay.h"
#include "tremo_regs.h"
#include <stdarg.h>
#include "tremo_uart.h"

void Byte_Write_BH1750(uint8_t data, uint32_t slave_addr)
{
	i2c_master_send_start(I2C0, slave_addr, I2C_WRITE);
		
		i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
		while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET)
			;
//		printf("send_start\r\n");
	
		// send data
		i2c_send_data(I2C0, data);

		i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
		while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET)
			;
	
		// stop
		i2c_master_send_stop(I2C0);
//		printf("send_stop\r\n");
}

void uart_log_init(void)
{
    // uart0
    gpio_set_iomux(GPIOB, GPIO_PIN_0, 1);
    gpio_set_iomux(GPIOB, GPIO_PIN_1, 1);

    /* uart config struct init */
    uart_config_t uart_config;
    uart_config_init(&uart_config);

	//uart_config.fifo_mode = ENABLE;
    uart_config.baudrate = UART_BAUDRATE_115200;
    uart_init(UART0, &uart_config);
    uart_cmd(UART0, ENABLE);
}

void Byte_Read_BH1750(uint8_t * data, uint32_t slave_addr)
{
	i2c_master_send_start(I2C0, slave_addr, I2C_READ);
	delay_ms(1);
//	printf("send_read_start\r\n");
	i2c_set_receive_mode(I2C0,I2C_ACK);
	while (i2c_get_flag_status(I2C0, I2C_FLAG_RECV_FULL) != SET)
            ;
	data[0] = i2c_receive_data(I2C0);
	i2c_clear_flag_status(I2C0, I2C_FLAG_RECV_FULL);
//	printf("receive_data0\r\n");
	
	i2c_set_receive_mode(I2C0,I2C_NAK);
	while (i2c_get_flag_status(I2C0, I2C_FLAG_RECV_FULL) != SET)
            ;
	data[1] = i2c_receive_data(I2C0);
	i2c_clear_flag_status(I2C0, I2C_FLAG_RECV_FULL);
//	printf("receive_data1\r\n");
	
    i2c_master_send_stop(I2C0);
//	printf("send_read_stop\r\n");
	delay_ms(5);
}

int main(void)
{
    uint32_t slave_addr = 0x23;
	uint8_t receive_data[2];
	uint8_t data;
	float temp1,temp2;
    i2c_config_t config;
	
	
    // enable the clk
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_I2C0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
	uart_log_init();
	
    // set iomux
    gpio_set_iomux(GPIOA, GPIO_PIN_14, 3);
    gpio_set_iomux(GPIOA, GPIO_PIN_15, 3);

    // init
    i2c_config_init(&config);
    i2c_init(I2C0, &config);
    i2c_cmd(I2C0, true);

    // send 0x01
    Byte_Write_BH1750(0x01,0x23);
    /* Infinite loop */
    while (1) {
		// send 0x10
		Byte_Write_BH1750(0x10,0x23);
		delay_ms(200);
		
		Byte_Read_BH1750(receive_data,slave_addr);
		data = receive_data[0];
		data = (data << 8) + receive_data[1];
		temp1=data/1.2;
		temp2=10*data/1.2;
		temp2=(int)temp2%10;
		
		printf("%f\r\n",temp1);
		delay_ms(2000);
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
