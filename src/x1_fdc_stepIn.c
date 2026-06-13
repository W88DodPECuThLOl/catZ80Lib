#include "x1_fdc_stepIn.h"
#include "x1_fdc_def.h"

void
x1_fdcStepIn() __naked
{
    //outp(FDC_PORT_CR, FDC_COMMAND_STEP_IN);
    __asm
    ld bc, #FDC_PORT_CR
    ld a, #FDC_COMMAND_STEP_IN
    out (c),a
    ret
    __endasm;
}
