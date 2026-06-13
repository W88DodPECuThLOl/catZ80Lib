#include "x1_disk_readRecord.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_disk_seek.h"
#include "x1_disk_waitForReady.h"
#include "x1_fdc_mortorOff.h"
#include "x1_fdc_mortorOn.h"
#include "x1_fdc_readData.h"
#include "x1_fdc_restore.h"

#define TIME_OUT_VALUE (0x25000U)

#if 0
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
            x1_diskSeek(trackNo, currentTrackNo);
            status = x1_diskWaitForReady(TIME_OUT_VALUE);
            if((status & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_BUSY)) == 0) {
                status = x1_fdcReadData(buffer, sectorNo);
            }
        }
    }
    x1_fdcMortorOff(driveNo);
    return status;
}
#else

extern X1_DISK_CONTEXT x1DiskContext;

const u8
x1_diskReadRecord(u8* buffer, const u8 driveNo, const u32 recordNo)
{
    u8 sectorNo = (recordNo & 0x000F) + 1;
    u8 side     = (recordNo >> 4) & 1;
    u8 trackNo  = (recordNo >> 5);

    // ドライブとサイド番号の選択
    u8 status = 0;
    if((x1DiskContext.driveNo != driveNo) || (x1DiskContext.side != side)) {
        x1_fdcMortorOn(driveNo, side);
        status = x1_diskWaitForReady(TIME_OUT_VALUE);
        if((status & (FDC_STATUS_TYPE1_NOT_READY | FDC_STATUS_TYPE1_BUSY))) {
            goto error;
        }
        // ドライブが違っていたらトラックをリストアしておく
        if(x1DiskContext.driveNo != driveNo) {
            x1_fdcRestore();
            status = x1_diskWaitForReady(TIME_OUT_VALUE);
            if(status & (FDC_STATUS_TYPE1_NOT_READY | FDC_STATUS_TYPE1_BUSY)) {
                goto error;
            }
            x1DiskContext.trackNo = 0;
        }
        x1DiskContext.driveNo = driveNo;
        x1DiskContext.side = side;
    }

    // トラック選択
    x1_diskSeek(trackNo, x1DiskContext.trackNo);
    status = x1_diskWaitForReady(TIME_OUT_VALUE);
    if((status & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_BUSY))) {
        goto error;
    }
    x1DiskContext.trackNo = trackNo;
    // 読み込み
    status = x1_fdcReadData(buffer, sectorNo);
    if(status & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
        goto error;
    }
    return status;

    // エラー処理
error:
    x1DiskContext.driveNo = 0xFF;
    x1DiskContext.side = 0xFF;
    x1DiskContext.trackNo = 0xFF;
    x1_fdcMortorOff(driveNo);
    return status;
}

#endif
