#include "x1_fdc_seek.h"
#include "x1_fdc_def.h"
//#include "z80_outp.h"

void
x1_fdcSeek(const u8 destinationTrackNo, const u8 currentTrackNo) __naked
{
//    outp(FDC_PORT_DR, destinationTrackNo);
//    outp(FDC_PORT_TR, currentTrackNo);
//    outp(FDC_PORT_CR, FDC_COMMAND_SEEK);

    (void)destinationTrackNo;
    (void)currentTrackNo;

    __asm
    ; a : destinationTrackNo
    ; l : currentTrackNo

    ld bc,#FDC_PORT_DR ; 0x0FFB
    out (c),a          ; destinationTrackNo
    ld c,#0xF9         ; 0x0FF9
    out (c),l          ; currentTrackNo
    dec c              ; 0x0FF8
    ld a,#FDC_COMMAND_SEEK
    out (c),a
    ret
    __endasm;
}
