#ifndef _RELAY_H_
#define _RELAY_H_

#include "tremo_gpio.h"
#include "stdio.h"
#include "gwlog.h"
#include <string.h>

void relay_gpio_init(gpio_t* gpiox, uint8_t gpio_pin);
void relay_on(gpio_t* gpiox, uint8_t gpio_pin);
void relay_off(gpio_t* gpiox, uint8_t gpio_pin);
bool relay_instruction_analysis(uint8_t gpioxNum, uint8_t gpio_pinNum);
void relay_read_pins_status(gpio_t* gpiox, uint8_t* AppData, uint8_t* AppDataSize);

#endif
