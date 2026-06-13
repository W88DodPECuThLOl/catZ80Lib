#include "x1_file_readFat.h"
#include "x1_fdc_def.h"
#include "x1_fs_def.h"
#include "x1_disk_readRecords.h"
#include "x1_disk_mortorOff.h"

u8
x1_fileReadFat_sub(const u8 driveNo)
{
    if(driveNo >= 4) {
        return FILE_SYSTEM_ERROR_INVALID_DRIVE_NO;
    }
    u8* fatBuffer = gFileSystemContext.fatBuffer;
    u8 state;
    const FileSystemDriveType driveType = gFileSystemContext.driveContext[driveNo].driveType;
    if(driveType == FILE_SYSTEM_DRIVE_TYPE_2D) {
        state = x1_diskReadRecords(fatBuffer, driveNo, FILE_SYSTEM_2D_FAT_RECORD_NO, FILE_SYSTEM_2D_FAT_RECORD_COUNT);
    } else if(driveType == FILE_SYSTEM_DRIVE_TYPE_2DD) {
        state = x1_diskReadRecords(fatBuffer, driveNo, FILE_SYSTEM_2DD_FAT_RECORD_NO, FILE_SYSTEM_2DD_FAT_RECORD_COUNT);
    } else {
        state = x1_diskReadRecords(fatBuffer, driveNo, FILE_SYSTEM_2HD_FAT_RECORD_NO, FILE_SYSTEM_2HD_FAT_RECORD_COUNT);
    }
    if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
        if(state & FDC_STATUS_TYPE23_NOT_READY) {
            return FILE_SYSTEM_ERROR_DEVICE_OFFLINE;
        }
        return FILE_SYSTEM_ERROR_READ;
    }
    return FILE_SYSTEM_SUCCESS;
}

u8
x1_fileReadFat(const u8 driveNo)
{
    u8 state = x1_fileReadFat_sub(driveNo);
    x1_diskMortorOff(driveNo);
    return state;
}
