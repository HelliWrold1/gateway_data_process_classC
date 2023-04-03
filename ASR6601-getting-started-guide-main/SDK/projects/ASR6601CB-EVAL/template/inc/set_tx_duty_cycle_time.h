#include "stdint.h"
#include <stdbool.h>
#include "gwlog.h"
#include <string.h>

bool parse_TxDutyCycleTime_instruction(uint32_t* TxDutyCycleTime, uint8_t cmd, uint8_t h, uint8_t m, uint8_t s, uint8_t* AppData, uint8_t* AppDataSize, bool* TxDutyCycleTimeQuery);