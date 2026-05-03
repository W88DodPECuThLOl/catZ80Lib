#include "x1_fdc_status.h"
#include "x1_fdc_def.h"
#include "z80_inp.h"

const u8
x1_fdcStatus()
{
    return inp(FDC_PORT_STR);
}
