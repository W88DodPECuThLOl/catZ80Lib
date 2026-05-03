#include "x1_disk_waitForNotBusy.h"
#include "x1_fdc_status.h"

u8
x1_diskWaitForNotBusy()
{
//    while((x1_fdcStatus() & 0x81) != 0) {}

    u8 stat;
    do {
        stat = x1_fdcStatus();
    } while((stat & 0x81) != 0);
    return stat;
}
