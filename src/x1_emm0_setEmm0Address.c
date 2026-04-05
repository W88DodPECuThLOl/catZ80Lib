#include "x1_emm0_setEmm0Address.h"
#include "x1_portAddress.h"
#include "z80_outp.h"

void
x1_setEmm0Address(const u32 emmAddress)
{
    outp(X1_PORT_EMM0 + 0,  emmAddress        & 0xFF);
    outp(X1_PORT_EMM0 + 1, (emmAddress >>  8) & 0xFF);
    outp(X1_PORT_EMM0 + 2, (emmAddress >> 16) & 0xFF);
}
