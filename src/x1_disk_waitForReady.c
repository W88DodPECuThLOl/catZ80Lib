#include "x1_disk_waitForReady.h"
#include "x1_fdc_status.h"

u8
x1_diskWaitForReady(u32 timeOut)
{
    u8 stat;
    do {
        stat = x1_fdcStatus();
        if(timeOut == 0) {
            return stat;
        }
        --timeOut;
    } while((stat & 0x81) != 0);
    return stat;
}
