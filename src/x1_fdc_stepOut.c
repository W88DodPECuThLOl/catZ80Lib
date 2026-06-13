#include "x1_fdc_stepOut.h"
#include "x1_fdc_def.h"

void
x1_fdcStepOut() __naked
{
    //outp(FDC_PORT_CR, FDC_COMMAND_STEP_OUT);
    __asm
    ld bc, #FDC_PORT_CR
    ld a, #FDC_COMMAND_STEP_OUT
    out (c),a
    ret
    __endasm;
}
