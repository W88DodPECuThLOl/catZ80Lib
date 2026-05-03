#include "x1_fdc_seek.h"
#include "x1_fdc_def.h"
#include "z80_outp.h"

void
x1_fdcSeek(const u8 destinationTrackNo, const u8 currentTrackNo)
{
    outp(FDC_PORT_DR, destinationTrackNo);
    outp(FDC_PORT_TR, currentTrackNo);
    outp(FDC_PORT_CR, FDC_COMMAND_SEEK);
}
