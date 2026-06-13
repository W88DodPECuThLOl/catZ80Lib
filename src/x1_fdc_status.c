#include "x1_fdc_status.h"
#include "x1_fdc_def.h"
#include "z80_inp.h"

const u8
x1_fdcStatus() __naked
{
    //return inp(FDC_PORT_STR);
    __asm
    ld bc,#FDC_PORT_STR
    in a,(c)
    ret
    __endasm;
}
