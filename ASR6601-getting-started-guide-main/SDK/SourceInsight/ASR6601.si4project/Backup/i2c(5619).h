#include "tremo_i2c.h"
#include "tremo_regs.h"
#include "tremo_delay.h"

void i2c_write_byte(i2c_t* i2cx, uint8_t data, uint32_t slave_addr);
void i2c_read_byte(i2c_t* i2cx, uint8_t data, uint32_t slave_addr);
void i2c_read_bytes_BH1750(i2c_t* i2cx, uint8_t* data, uint32_t slave_addr);
double request_data_BH1750(i2c_t* i2cx, uint32_t slave_addr);