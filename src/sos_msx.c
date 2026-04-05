#include "sos_msx.h"

void
sos_msx(const char* pszMessage) __naked
{
    (void)pszMessage;

    // hl : mapper
    __asm
    ex   de,hl
    call #0x1FE5 ; MSX
    ret
    __endasm;
}
