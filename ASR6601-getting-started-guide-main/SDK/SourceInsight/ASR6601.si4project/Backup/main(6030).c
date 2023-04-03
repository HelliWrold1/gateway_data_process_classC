#include <stdio.h>
#include "tremo_rcc.h"
#include "tremo_gpio.h"
#include "tremo_i2c.h"
#include "tremo_delay.h"
#include "tremo_regs.h"

void Byte_Write_BH1750(uint8_t data, uint32_t slave_addr)
{
	i2c_master_send_start(I2C0, slave_addr, I2C_WRITE);
		
		i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
		while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET)
			;
	
		// send data
		for(int i=0; i<2; i++) {
			i2c_send_data(I2C0, data[i]);
	
			i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
			while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET)
				;
		}
	
		// stop
		i2c_master_send_stop(I2C0);
}

//void Byte_Read_BH1750(uint8_t data, uint32_t slave_addr)
//{
//	i2c_master_send_start(I2C0, slave_addr, I2C_WRITE);
//	while(I2C_CR_ACKNAK_MASK != 1);
//	i2c_send_data(I2C0, 0x01);
//	while(I2C_CR_ACKNAK_MASK != 1);
//	i2c_set_receive_mode(I2C0,I2C_ACK);
//    i2c_master_send_start(I2C0, slave_addr, I2C_READ);
//	while(I2C_CR_ACKNAK_MASK != 1);
//    data = i2c_receive_data(I2C0);
//    i2c_master_send_stop(I2C0);
//}

void Byte_Read_BH1750(uint8_t * data, uint32_t slave_addr)
{
	i2c_master_send_start(I2C0, slave_addr, I2C_READ);
	while(I2C_CR_ACKNAK_MASK != 1);
	i2c_set_receive_mode(I2C0,I2C_ACK);
	while(I2C_CR_ACKNAK_MASK != 1);
	data[0] = i2c_receive_data(I2C0);
	I2C_CR_ACKNAK_MASK = 1;
	data[1] = i2c_receive_data(I2C0);
	I2C_CR_ACKNAK_MASK = 0;
    i2c_master_send_stop(I2C0);
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

    // set iomux
    gpio_set_iomux(GPIOA, GPIO_PIN_14, 3);
    gpio_set_iomux(GPIOA, GPIO_PIN_15, 3);

    // init
    i2c_config_init(&config);
    i2c_init(I2C0, &config);
    i2c_cmd(I2C0, true);

    // send 0x01
    Byte_Write_BH1750(0x01,0x23);
	// send 0x10
	Byte_Write_BH1750(0x10,0x23);
	delay_ms(200);
	
	Byte_Read_BH1750(receive_data,slave_addr);
	data = receive_data[0];
	data = (data << 8) + receive_data[1];
	temp1=data/1.2;
	temp2=10*data/1.2;
	temp2=(int)temp2%10;
	
    /* Infinite loop */
    while (1) { }
}

#ifdef USE_FULL_ASSERT
void assert_failed(void* file, uint32_t line)
{
    (void)file;
    (void)line;

    while (1) { }
}
#endif
