#ifndef LINKC_PACKAGE
#define LINKC_PACKAGE
#include <stdint.h>

struct PackageHeader_t{
	uint8_t		Type;
	uint8_t		Version;
	uint16_t	Length;
	uint32_t	Count;
};

struct PackageBody_t{
	uint8_t		MessageType;
	uint8_t		Total;
	uint8_t		Current;
};

#endif

