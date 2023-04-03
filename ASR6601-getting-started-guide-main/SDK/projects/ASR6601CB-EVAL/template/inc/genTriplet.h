#ifndef _GENTRIPLET_H_
#define _GENTRIPLET_H_

#include "sha256.h"
#include "tremo_system.h"
#include "gwlog.h"

void genTriplet(uint32_t *DevAddr, uint8_t *NwkSKey, uint8_t *AppSKey);

#endif