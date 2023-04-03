#include "relay.h"

/**
 * \brief Init relay gpio pin
 * \param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * \param gpio_pin Select the GPIO pin number. \ref tremo_gpio.h
 * \retval None
 * \author ZhaoZeqiang
 */
void relay_gpio_init(gpio_t* gpiox, uint8_t gpio_pin)
{
	gpio_set_iomux(gpiox, gpio_pin, 0);
	gpio_init(gpiox, gpio_pin, GPIO_MODE_OUTPUT_PP_LOW);
}

/**
 * \brief Give the relay pin a high level
 * \param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * \param gpio_pin Select the GPIO pin number. \ref tremo_gpio.h 
 * \retval None
 * \author ZhaoZeqiang
 */
void relay_on(gpio_t* gpiox, uint8_t gpio_pin)
{
	gpio_write(gpiox,gpio_pin,GPIO_LEVEL_HIGH);
}

/**
 * \brief Give the relay pin a low level
 * \param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * \param gpio_pin Select the GPIO pin number. \ref tremo_gpio.h
 * \retval None
 * \author ZhaoZeqiang
 */
void relay_off(gpio_t* gpiox, uint8_t gpio_pin)
{
	gpio_write(gpiox,gpio_pin,GPIO_LEVEL_LOW);
}

/**
 * \brief Relay instruction analysis
 * \param gpioxNum [such as:0xfa,0xfd]gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * \param gpio_pin [such as:0x00,0xff]Select the GPIO pin number. Write the high or low level
 * \retval Indicates whether the command has been executed[true: executed,false:error]
 * \author ZhaoZeqiang
 */
bool relay_instruction_analysis(uint8_t gpioxNum, uint8_t gpio_pinNum)
{
	uint8_t gpioxHigh, gpioxLow;
	uint8_t gpio_pinHigh, gpio_pinLow;
	gpio_t* gpiox;
	
	gpioxLow = gpioxNum & 0x0f;
	gpioxHigh = gpioxNum & 0xf0;
	gpio_pinLow = gpio_pinNum & 0x0f;
	gpio_pinHigh = (gpio_pinNum & 0xf0) >> 4;
	GW_LOG(LOG_DEBUG, "pin: %d status: %d", gpio_pinHigh, (gpio_pinLow == 0x0f ? 1 : 0));  // pin
	
	// gpioxHigh must be 0xf0
	if(gpioxHigh == 0xf0)
	{
		switch (gpioxLow)
		{
			case 0x0a:
			{
				gpiox = GPIOA;
				break;
			}
			case 0x0b:
			{
				gpiox = GPIOB;
				break;
			}
			case 0x0c:
			{
				gpiox = GPIOC;
				break;
			}
			case 0x0d:
			{
				gpiox = GPIOD;
				break;
			}
			default:
				return false;
		}
	}
	else
	{
		return false;
	}

	// Select the GPIO pin number
//	if(gpio_pinHigh < 0x10)
	if( ( (gpio_pinHigh >= 4)&&(gpio_pinHigh <= 9) ) || gpio_pinHigh == 11 || gpio_pinHigh == 14 \ 
			|| gpio_pinHigh == 15 || gpio_pinHigh == 16 || gpio_pinHigh == 17 || gpio_pinHigh == 60 )
	{
		// Write the high or low level
		if(gpio_pinLow == 0x00)
		{
			gpio_write(gpiox,gpio_pinHigh,GPIO_LEVEL_LOW);
		}
		else if(gpio_pinLow == 0x0f)
		{
			gpio_write(gpiox,gpio_pinHigh,GPIO_LEVEL_HIGH);
		}
		else
		{
			return false;
		}	
	}
	else
	{
		return false;
	}
	return true;
}

/**
 * \brief Read GPIO pins status, write the AppData and AppDataSize
 * \param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * \param AppData User application data
 * \param AppDataSize Point to user application data size variable
 * \retval Indicates whether the command has been executed[true: executed,false:error]
 * \author ZhaoZeqiang
 */
void relay_read_pins_status(gpio_t* gpiox, uint8_t* AppData, uint8_t* AppDataSize)
{
	memset(AppData, 0, 16);
	GW_LOG(LOG_DEBUG, "read:%X",GPIOA->ODR);
	AppData[0] = 0x01; // DataType Control
	AppData[2] = (gpiox->ODR & 0x0010) >> 4; // io4 statu
	AppData[4] = (gpiox->ODR & 0x0020) >> 5;
	AppData[6] = (gpiox->ODR & 0x0100) >> 8;
	AppData[8] = (gpiox->ODR & 0x0200) >> 9;
	AppData[10] = (gpiox->ODR & 0x0800) >> 11;
	AppData[12] = (gpiox->ODR & 0x4000) >> 14;
	AppData[15] = (gpiox->ODR & 0x8000) >> 15; // io15
	*AppDataSize = 16;
}
