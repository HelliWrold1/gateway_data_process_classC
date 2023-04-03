#include "i2c.h"

/**
 * @brief Send data to slave_addr
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param data The data will be send
 * @param slave_addr The slave address
 * @retval None
 */
void i2c_write_byte(i2c_t* i2cx, uint8_t data, uint32_t slave_addr)
{
	i2c_master_send_start(i2cx, slave_addr, I2C_WRITE);
		
	i2c_clear_flag_status(i2cx, I2C_FLAG_TRANS_EMPTY);
	while (i2c_get_flag_status(i2cx, I2C_FLAG_TRANS_EMPTY) != SET);

	// send data
	i2c_send_data(i2cx, data);

	i2c_clear_flag_status(i2cx, I2C_FLAG_TRANS_EMPTY);
	while (i2c_get_flag_status(i2cx, I2C_FLAG_TRANS_EMPTY) != SET);

	// stop
	i2c_master_send_stop(i2cx);
}

/**
 * @brief Read data from slave_addr
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param data The data, this function will pass the slave data into this parameter
 * @param slave_addr The slave address
 * @retval None
 */
void i2c_read_byte(i2c_t* i2cx, uint8_t data, uint32_t slave_addr)
{
	
}

/**
 * @brief Read data from BH1750
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param data The data, this function will pass the slave data into this parameter
 * @param slave_addr The slave address
 * @retval None
 */
void i2c_read_bytes_BH1750(i2c_t* i2cx, uint8_t* data, uint32_t slave_addr)
{
	i2c_master_send_start(i2cx, slave_addr, I2C_READ);
	delay_ms(5);
	
	i2c_set_receive_mode(i2cx,I2C_ACK);
	while (i2c_get_flag_status(i2cx, I2C_FLAG_RECV_FULL) != SET);
	data[0] = i2c_receive_data(i2cx);
	i2c_clear_flag_status(i2cx, I2C_FLAG_RECV_FULL);
	
	i2c_set_receive_mode(i2cx,I2C_NAK);
	while (i2c_get_flag_status(i2cx, I2C_FLAG_RECV_FULL) != SET);
	data[1] = i2c_receive_data(i2cx);
	i2c_clear_flag_status(i2cx, I2C_FLAG_RECV_FULL);
	
    i2c_master_send_stop(i2cx);
	delay_ms(5);
}

/**
 * @brief Request data from BH1750
 * @param i2cx Select the I2C peripheral number(I2C0, I2C1 and I2C2)
 * @param slave_addr The slave address
 * @retval result The light intensity collected by BH1750
 */
double request_data_BH1750(i2c_t* i2cx, uint32_t slave_addr)
{
	uint8_t receive_data[2];
	uint8_t temp;
	double result;
	
	i2c_write_byte(i2cx,0x01,0x23);
	// send 0x10 to request data
	i2c_write_byte(i2cx, 0x10, 0x23);
	delay_ms(200); // wait result
	
	i2c_read_bytes_BH1750(i2cx, receive_data, slave_addr);
	// caculate result according to the receive data
	temp = receive_data[0];
	temp = (temp << 8) + receive_data[1];
	result = temp/1.2;
	return result;
}