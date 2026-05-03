#include "x1_fdc_mortorOff.h"
#include "x1_fdc_def.h"
#include "z80_outp.h"

void
x1_fdcMortorOff(const u8 driveNo)
{
    outp(FDC_PORT_DSM, (driveNo & 0x03));
}
