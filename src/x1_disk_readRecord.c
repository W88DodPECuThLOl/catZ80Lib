#include "x1_disk_readRecord.h"
#include "x1_fdc_def.h"
#include "x1_disk_waitForReady.h"
#include "x1_fdc_mortorOff.h"
#include "x1_fdc_mortorOn.h"
#include "x1_fdc_readData.h"
#include "x1_fdc_restore.h"
#include "x1_fdc_seek.h"

#define TIME_OUT_VALUE (0x25000U)

const u8
x1_diskReadRecord(u8* buffer, const u8 driveNo, const u32 recordNo)
{
    u8 sectorNo = (recordNo & 0x000F) + 1;
    u8 side     = (recordNo >> 4) & 1;
    u8 trackNo  = (recordNo >> 5);

    x1_fdcMortorOn(driveNo, side);
    u8 status = x1_diskWaitForReady(TIME_OUT_VALUE);
    if((status & (FDC_STATUS_TYPE1_NOT_READY | FDC_STATUS_TYPE1_BUSY)) == 0) {
        x1_fdcRestore();
        status = x1_diskWaitForReady(TIME_OUT_VALUE);
        if((status & (FDC_STATUS_TYPE1_NOT_READY | FDC_STATUS_TYPE1_BUSY)) == 0) {
            u8 currentTrackNo = 0;
            x1_fdcSeek(trackNo, currentTrackNo);
            status = x1_diskWaitForReady(TIME_OUT_VALUE);
            if((status & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_BUSY)) == 0) {
                status = x1_fdcReadData(buffer, sectorNo);
            }
        }
    }
    x1_fdcMortorOff(driveNo);
    return status;
}
