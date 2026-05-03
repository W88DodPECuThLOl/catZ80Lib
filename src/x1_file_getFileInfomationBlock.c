#include "x1_file_getFileInfomationBlock.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_fs_def.h"
#include "x1_disk_readRecord.h"
#include "x1_file_getDriveNoFromFilename.h"

#include <string.h>

#define END_MARK     (0xFF)
#define DELETED_MARK (0x00)

static u8
normalizeFileNames(char* normalizedFilename, const u8* filename)
{
    char* dst = normalizedFilename;
    char* dstEnd = dst + FILE_SYSTEM_FILE_NAME_LENGTH + FILE_SYSTEM_FILE_EXTENTION_LENGTH;
    while(dst != dstEnd) {
        *dst++ = 0x20;
    }
    *dst = 0;

    // ドライブ名を飛ばす
    const char* src = filename;
    while(*src != 0 && *src != ':') { src++; }
    if(*src == 0) {
        // ドライブ名が無かった
        src = filename;
    } else {
        src++;
    }
    // ファイル名をコピー
    dst = normalizedFilename;
    dstEnd = dst + FILE_SYSTEM_FILE_NAME_LENGTH;
    while(*src != 0 && *src != '.' && dst != dstEnd) {
        *dst++ = *src++;
    }
    if(*src == '.') {
        src++;
    } else if(*src == 0) {
        return FILE_SYSTEM_SUCCESS;
    } else if(dst == dstEnd) {
        return FILE_SYSTEM_ERROR_BAD_FILE_NAME;
    }
    // 拡張子をコピー
    dst = normalizedFilename + FILE_SYSTEM_FILE_NAME_LENGTH;
    dstEnd = dst + FILE_SYSTEM_FILE_EXTENTION_LENGTH;
    while(*src != 0 && dst != dstEnd) {
        *dst++ = *src++;
    }
    if(*src != 0) {
        return FILE_SYSTEM_ERROR_BAD_FILE_NAME;
    }
    return FILE_SYSTEM_SUCCESS;
}

u8
x1_fileGetFileInfomationBlock(const char* filename, u8* infomationBlock)
{
    u8 driveNo;
    u8 rc = x1_fileGetDriveNoFromFilename(&driveNo, gFileSystemContext.currentDriveNo, filename);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }
//sos_printf("x1_fileGetDriveNoFromFilename : %d\r", driveNo);
    char normalizedFilename[FILE_SYSTEM_FILE_NAME_LENGTH+FILE_SYSTEM_FILE_EXTENTION_LENGTH+1];
    rc = normalizeFileNames(normalizedFilename, filename);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }
//sos_printf("normalizeFileNames : %s\r", normalizedFilename);

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

            if(ib[0] == END_MARK) {
                // 最後まで検索した
                return FILE_SYSTEM_ERROR_FILE_NOT_FOUND;
            }
            if(ib[0] != DELETED_MARK) {
                if(memcmp(normalizedFilename, ib + 1, FILE_SYSTEM_FILE_NAME_LENGTH+FILE_SYSTEM_FILE_EXTENTION_LENGTH) == 0) {
                    memcpy(infomationBlock, ib, FILE_SYSTEM_IB_SIZE);
                    return FILE_SYSTEM_SUCCESS;
                }
            }
            ib += FILE_SYSTEM_IB_SIZE;
        }
    }
    return FILE_SYSTEM_ERROR_FILE_NOT_FOUND;
}
