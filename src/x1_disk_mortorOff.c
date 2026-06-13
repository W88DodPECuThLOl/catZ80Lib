#include "x1_disk_mortorOff.h"
#include "x1_disk_def.h"
#include "x1_fdc_mortorOff.h"

extern X1_DISK_CONTEXT x1DiskContext;

void
x1_diskMortorOff(const u8 driveNo)
{
    x1DiskContext.driveNo = 0xFF;
    x1DiskContext.side = 0xFF;
    x1DiskContext.trackNo = 0xFF;
    x1_fdcMortorOff(driveNo);
}
