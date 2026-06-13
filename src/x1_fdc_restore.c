#include "x1_fdc_restore.h"
#include "x1_fdc_def.h"

void x1_fdcRestore() __naked
{
    //outp(FDC_PORT_CR, FDC_COMMAND_RESTORE);
    __asm
    ld bc,#FDC_PORT_CR
    ld a,#FDC_COMMAND_RESTORE
    out (c),a
    ret
    __endasm;
}
