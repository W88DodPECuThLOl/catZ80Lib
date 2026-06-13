#include "x1_disk_initialize.h"
#include "x1_disk_def.h"
#include "x1_fdc_mortorOff.h"

X1_DISK_CONTEXT x1DiskContext;

void
x1_diskInitialize()
{
    x1DiskContext.driveNo = 0xFF;
    x1DiskContext.side = 0xFF;
    x1DiskContext.trackNo = 0xFF;
    x1_fdcMortorOff(3);
    x1_fdcMortorOff(2);
    x1_fdcMortorOff(1);
    x1_fdcMortorOff(0);
}
