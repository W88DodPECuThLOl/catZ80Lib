#include "x1_fdc_mortorOn.h"
#include "x1_fdc_def.h"
#include "z80_outp.h"

void
x1_fdcMortorOn(const u8 driveNo, const u8 side)
{
    outp(FDC_PORT_DSM, (driveNo & 0x03) | ((side & 1) << 4) | 0x80);
}
