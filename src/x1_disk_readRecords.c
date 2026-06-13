#include "x1_disk_readRecords.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_disk_readRecord.h"

const u8
x1_diskReadRecords(u8* buffer, const u8 driveNo, const u32 recordNo, const u8 readRecordCount)
{
    u8 status = 0;
    for(u8 i = 0; i < readRecordCount; ++i) {
        status = x1_diskReadRecord(buffer, driveNo, recordNo + i);
        if(status & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
            return status;
        }
        buffer += DISK_SECTOR_SIZE;
    }
    return status;
}
