/*  LinkC_Server Only Functions */
#include "../../include/linkc_server.h"
#include <stdint.h>

uint8_t GetActionType(uint8_t Type){
    return Type&0xF0;
}

uint8_t GetDataType(uint8_t Type){
    return Type&0x0F;
}
