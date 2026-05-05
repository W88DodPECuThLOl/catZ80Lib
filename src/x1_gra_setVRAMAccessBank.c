#include "x1_gra_setVRAMAccessBank.h"

__sfr __banked __at(0x1FD0) IoPort_1FD0;

void
x1_graSetVRAMAccessBank0()
{
    IoPort_1FD0 = (IoPort_1FD0 & 0xEF);
}

void
x1_graSetVRAMAccessBank1()
{
    IoPort_1FD0 = (IoPort_1FD0 | 0x10);
}
