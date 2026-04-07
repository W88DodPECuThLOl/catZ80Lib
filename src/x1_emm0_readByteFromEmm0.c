#include "x1_emm0_readByteFromEmm0.h"
#include "x1_emm0_setEmm0Address.h"
#include "z80_inp.h"
#include "x1_portAddress.h"

u8
x1_readByteFromEmm0(const u32 emmAddress)
{
    x1_setEmm0Address(emmAddress);
    return inp(X1_PORT_EMM0 + 3);
}
