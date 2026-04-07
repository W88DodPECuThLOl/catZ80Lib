#include "sos_loc.h"

void
sos_loc(u16 pos) __naked
{
    (void)pos;

    // hl : pos
    __asm
    call #0x201E
    ret
    __endasm;
}
