#include "x1_file_getFileInfomationBlock.h"
#include "x1_file_getDriveNoFromFilename.h"
#include "x1_file_normalizeFileName.h"
#include "x1_file_enumerateInfomationBlock.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_fs_def.h"
#include "x1_disk_readRecord.h"
#include "x1_disk_mortorOff.h"

#include <string.h>

static u8
compareFilename(const u8* infomationBlock, void* userData1, void* userData2, void* userData3)
{
    if(memcmp(userData1, infomationBlock + 1, FILE_SYSTEM_FILE_NAME_LENGTH+FILE_SYSTEM_FILE_EXTENTION_LENGTH) == 0) {
        memcpy(userData2, infomationBlock, FILE_SYSTEM_IB_SIZE);
        *(u8*)userData3 = FILE_SYSTEM_SUCCESS;
        return 1; // 終了
    }
    return 0; // 継続
}

u8
x1_fileGetFileInfomationBlock_sub(u8 driveNo, const char* filename, u8* infomationBlock)
{
    // ファイル名をノーマライズ
    char normalizedFilename[FILE_SYSTEM_FILE_NAME_LENGTH+FILE_SYSTEM_FILE_EXTENTION_LENGTH+1];
    u8 rc = x1_fileNormalizeFileName(normalizedFilename, filename);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }

    u8 result = FILE_SYSTEM_ERROR_FILE_NOT_FOUND;
    rc = x1_fileEnumerateInfomationBlock(driveNo, compareFilename, normalizedFilename, infomationBlock, &result);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }
    return result;
#if 0
    // IBを検索する
    u32 recordNo = FILE_SYSTEM_IB_RECORD_NO;
    u32 endRecordNo = FILE_SYSTEM_IB_RECORD_NO + 16;
    for(; recordNo != endRecordNo; ++recordNo) {
        u8* ibBuffer = gFileSystemContext.readWriteBuffer;
        u8 state = x1_diskReadRecord(ibBuffer, driveNo, recordNo);
        if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
            if(state & FDC_STATUS_TYPE23_NOT_READY) {
                return FILE_SYSTEM_ERROR_DEVICE_OFFLINE;
            }
            return FILE_SYSTEM_ERROR_READ;
        }
        u8* ib = ibBuffer;
        for(u8 i = 0; i < (DISK_SECTOR_SIZE / FILE_SYSTEM_IB_SIZE); ++i) {

            if(ib[0] == X1_FILE_FAT_END_MARK) {
                // 最後まで検索した
                return FILE_SYSTEM_ERROR_FILE_NOT_FOUND;
            }
            if(ib[0] != X1_FILE_FAT_DELETED_MARK) {
                if(memcmp(normalizedFilename, ib + 1, FILE_SYSTEM_FILE_NAME_LENGTH+FILE_SYSTEM_FILE_EXTENTION_LENGTH) == 0) {
                    memcpy(infomationBlock, ib, FILE_SYSTEM_IB_SIZE);
                    return FILE_SYSTEM_SUCCESS;
                }
            }
            ib += FILE_SYSTEM_IB_SIZE;
        }
    }
    return FILE_SYSTEM_ERROR_FILE_NOT_FOUND;
#endif
}

u8
x1_fileGetFileInfomationBlock(const char* filename, u8* infomationBlock)
{
    // ファイル名からドライブ番号を取得
    u8 driveNo;
    u8 rc = x1_fileGetDriveNoFromFilename(&driveNo, gFileSystemContext.currentDriveNo, filename);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }

    // IBを取得
    rc = x1_fileGetFileInfomationBlock_sub(driveNo, filename, infomationBlock);

    // ドライブのモーターを切っておく
    x1_diskMortorOff(driveNo);
    return rc;
}
