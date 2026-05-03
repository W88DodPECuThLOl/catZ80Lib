#include "x1_file_readFile.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_fs_def.h"
#include "x1_disk_readRecord.h"
#include "x1_file_getDriveNoFromFilename.h"
#include "x1_file_getFileInfomationBlock.h"
#include "x1_file_readFat.h"

#include <string.h>

/**
 * @brief 次のFATが無く、ここで終わりなのかどうかの判定
 */
static inline u8
IS_END_FAT(const u8 fatValue)
{
    return ((0x80 <= fatValue) && (fatValue <= 0x8F)) ? 1 : 0;
}

/**
 * @brief FATの値からクラスタ内で使用しているセクター数を取得する
 * @param fatValue FATの値
 * @return クラスタ内で使用しているセクター数
 * @retval 0 : エラー
 * @retval それ以外 : クラスタ内で使用しているセクター数
 */
static inline u8
getNumberOfSectorsUsedInTheCluster(const u8 fatValue)
{
    if(fatValue == 0x00) {
        return 0;
    }
    if(fatValue <= 0x7F) {
        // 0x01～0x7F
        return 16;
    }
    if(fatValue <= 0x8F) {
        // 0x80～0x8F
        return fatValue - 0x7F;
    }
    return 0;
}

static inline u32
getMaxCluster(const FileSystemDriveType driveType)
{
    if(driveType == FILE_SYSTEM_DRIVE_TYPE_2D) {
        return FILE_SYSTEM_2D_FAT_RECORD_COUNT * 128;
    } else if(driveType == FILE_SYSTEM_DRIVE_TYPE_2DD) {
        return FILE_SYSTEM_2DD_FAT_RECORD_COUNT * 128;
    } else {
        return FILE_SYSTEM_2HD_FAT_RECORD_COUNT * 128;
    }
}

u8
x1_fileReadFile(const char* filename, u8* buffer)
{
    u8 driveNo;
    u8 rc = x1_fileGetDriveNoFromFilename(&driveNo, gFileSystemContext.currentDriveNo, filename);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }

    // IB取得
    u8 infomationBlock[FILE_SYSTEM_IB_SIZE];
    rc = x1_fileGetFileInfomationBlock(filename, infomationBlock);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }

    // read FAT
    rc = x1_fileReadFat(driveNo);
    if(rc != FILE_SYSTEM_SUCCESS) {
        return rc;
    }

    u32 dataSize = x1_fileGetInfomationBlockFileSize(infomationBlock);
    u32 cluster = x1_fileGetInfomationBlockStartCluster(infomationBlock);

    const FileSystemDriveType driveType = gFileSystemContext.driveContext[driveNo].driveType;
    const u32 maxCluster = getMaxCluster(driveType);

    while(1) {
//sos_printf("cluster:%d\r", (u16)cluster);
        if(cluster >= maxCluster) {
            // クラスタが範囲外
            return FILE_SYSTEM_ERROR_READ;
        }

        const u16 index = (cluster & 0x7F) + DISK_SECTOR_SIZE * (cluster >> 7);
        const u8 fatLow  = gFileSystemContext.fatBuffer[index];
        const u8 fatHigh = gFileSystemContext.fatBuffer[index + 0x80];

        const u8 readSectorCount = getNumberOfSectorsUsedInTheCluster(fatLow);
//sos_printf("fatLow:%02X\r", fatLow);
//sos_printf("readSectorCount:%d\r", readSectorCount);
        if(readSectorCount == 0) {
            return FILE_SYSTEM_ERROR_READ;
        }

        const u32 recordNo = cluster * 16;
        for(u8 i = 0; i < readSectorCount; ++i) {
            if(dataSize >= DISK_SECTOR_SIZE) {
                // 直接バッファへ読み込む
                u8 state = x1_diskReadRecord(buffer, driveNo, recordNo + i);
                if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
                    return FILE_SYSTEM_ERROR_READ;
                }
                buffer += DISK_SECTOR_SIZE;
                dataSize -= DISK_SECTOR_SIZE;
            } else {
                // 一時バッファへ読み込んでから転送する
                u8* tempBuffer = gFileSystemContext.readWriteBuffer;
                u8 state = x1_diskReadRecord(tempBuffer, driveNo, recordNo + i);
                if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
                    return FILE_SYSTEM_ERROR_READ;
                }
                memcpy(buffer, tempBuffer, dataSize);
                buffer += dataSize;
                dataSize = 0;
            }
        }

        if(IS_END_FAT(fatLow)) {
            return FILE_SYSTEM_SUCCESS; // 成功
        }
        if(driveType == FILE_SYSTEM_DRIVE_TYPE_2D) {
            cluster = fatLow & 0x7F;
        } else {
            cluster = (u16)(fatLow & 0x7F) | ((u16)fatHigh << 7);
        }
    }
}
