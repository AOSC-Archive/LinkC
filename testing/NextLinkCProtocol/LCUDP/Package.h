#ifndef LINKC_PACKAGE
#define LINKC_PACKAGE
#include <stdint.h>

struct PackageHeader_t{
    
    uint8_t     Type;
    uint8_t     Version;
    uint16_t    Length;
    uint32_t    Count;
};                      //  8 Byte Total
//  这一段的信息将以明文传输，后面的则要加密[如果支持的话]

struct PackageBody_t{
    uint8_t     Total;
    uint8_t     Current;
    uint16_t    MessageType;
};

typedef struct PackageHeader_t  PackageHeader;
typedef struct PackageBody_t    PackageBody;

#endif

