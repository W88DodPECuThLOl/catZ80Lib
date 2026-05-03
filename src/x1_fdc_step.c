#include "x1_fdc_step.h"
#include "x1_fdc_def.h"

void
x1_fdcStep()
{
    //outp(FDC_PORT_CR, FDC_COMMAND_STEP);
    __asm
    ld bc, #FDC_PORT_CR
    ld a, #FDC_COMMAND_STEP
    out (c),a
    __endasm;
}
