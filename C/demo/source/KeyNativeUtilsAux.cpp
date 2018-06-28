#include "KeyNativeUtilsAux.h"
#include <memory.h>

unsigned int getKeyElements(unsigned char* pu1KeyElement1, unsigned int* pu4KeyElement1, unsigned char* pu1KeyElement2, unsigned int* pu4KeyElement2)
{
	if (*pu4KeyElement1 < 0x08 || *pu4KeyElement2 < 0x08)
	{
		return 0xFF;
	}

	*pu4KeyElement1 = 0x08;
	memcpy(pu1KeyElement1, "\x11\x22\x33\x44\x55\x66\x77\x88", *pu4KeyElement1);
	*pu4KeyElement2 = 0x08;
	memcpy(pu1KeyElement2, "\x88\x77\x66\x55\x44\x33\x22\x11", *pu4KeyElement2);

	return 0x00;
}

unsigned int getKey(unsigned char* pu1KeyElement1, unsigned int u4KeyElement1Len, unsigned char* pu1KeyElement2, unsigned int u4KeyElement2Len,
					unsigned char* pu1Key, unsigned int* pu4KeyLen)
{
	if (*pu4KeyLen < (u4KeyElement1Len + u4KeyElement2Len))
	{
		return 0xFF;
	}
	memcpy(pu1Key, pu1KeyElement1, u4KeyElement1Len);
	memcpy(pu1Key + u4KeyElement1Len, pu1KeyElement2, u4KeyElement2Len);
	*pu4KeyLen = u4KeyElement1Len + u4KeyElement2Len;

	return 0x00;
}