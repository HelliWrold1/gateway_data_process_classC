#include "genTriplet.h"

void genTriplet(uint32_t *DevAddr, uint8_t *NwkSKey, uint8_t *AppSKey)
{
	uint32_t ChipId[2] = {0};
	(void)system_get_chip_id(ChipId);
	GW_LOG(LOG_DEBUG, "chipid: %X %X", ChipId[0], ChipId[1]);
	
	uint8_t hash[SHA256_DIGEST_SIZE];
	uint8_t addr[4];
    uint8_t bytes[8];
	
	uint32ToBytes(ChipId[0], ChipId[1], bytes);
    sha256(bytes, sizeof(bytes), hash);
	memcpy(addr, hash, 4);
	
	*DevAddr = 0;
	*DevAddr |= addr[0];
	*DevAddr = *DevAddr<<8 | addr[1];
	*DevAddr = *DevAddr<<8 | addr[2];
	*DevAddr = *DevAddr<<8 | addr[3];
	
	GW_LOG(LOG_DEBUG, "DevAddr: %08X", *DevAddr);
	
	#ifdef OPEN_LOG
	if (LOG_LEVEL == LOG_DEBUG)
	{
		printf("addr: ");
		int count = 0;
		for (int i = 0; i < 4; i++) 
		{
			printf("%02X", hash[i]);
			count++;
		}
		printf("\r\n");
		GW_LOG(LOG_DEBUG, "count: %d", count);
	}
	#endif
	
    uint32ToBytes(ChipId[1], ChipId[0], bytes);
    sha256(bytes, sizeof(bytes), hash);
	memcpy(NwkSKey, hash, 16);
    memcpy(AppSKey, hash + 16, 16);
	#ifdef OPEN_LOG
	if (LOG_LEVEL == LOG_DEBUG)
	{
		printf("NwkSKey: ");
		printHash(NwkSKey);
		printf("AppSKey: ");
		printHash(AppSKey);
	}
	#endif
}