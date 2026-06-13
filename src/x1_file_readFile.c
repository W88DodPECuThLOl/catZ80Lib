#include "x1_file_readFile.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_fs_def.h"
#include "x1_disk_readRecord.h"
#include "x1_disk_readRecords.h"
#include "x1_file_getDriveNoFromFilename.h"
#include "x1_file_getFileInfomationBlock.h"
#include "x1_file_readFat.h"

#include <string.h>

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
    u32 startClusterNo = x1_fileGetInfomationBlockStartCluster(infomationBlock);
    u8 clusterNo = startClusterNo & 0x7F;
    while(1) {
        u8 temp = gFileSystemContext.fatBuffer[clusterNo & 0x7F];
        if(temp == 0) {
            // 未使用
            return FILE_SYSTEM_ERROR_READ;
        } else if(temp <= 0x7F) {
            // 次に続く
            {
                // 1クラスタ丸々読み込む
                const u32 recordNo = clusterNo * 16;
                u8 state = x1_diskReadRecords(buffer, driveNo, recordNo, 16);
                if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
                    return FILE_SYSTEM_ERROR_READ;
                }
                buffer += DISK_SECTOR_SIZE*16;
                dataSize -= DISK_SECTOR_SIZE*16;
            }
            clusterNo = temp;
            continue;
        } else if(temp <= 0x8F) {
            u32 recordNo = clusterNo * 16;
            const u8 recoredSize = dataSize / DISK_SECTOR_SIZE;
            if(recoredSize > 0) {
                // 纏めて読めるのは読み込む
                u8 state = x1_diskReadRecords(buffer, driveNo, recordNo, recoredSize - 1);
                if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
                    return FILE_SYSTEM_ERROR_READ;
                }
                buffer += DISK_SECTOR_SIZE*recoredSize;
                dataSize -= DISK_SECTOR_SIZE*recoredSize;
                recordNo += recoredSize;
            }
            if(dataSize > 0) {
                // 一時バッファへ読み込んでから転送する
                u8* tempBuffer = gFileSystemContext.readWriteBuffer;
                u8 state = x1_diskReadRecord(tempBuffer, driveNo, recordNo);
                if(state & (FDC_STATUS_TYPE23_NOT_READY | FDC_STATUS_TYPE23_RECORD_NOT_FOUND | FDC_STATUS_TYPE23_CRC_ERROR | FDC_STATUS_TYPE23_BUSY)) {
                    return FILE_SYSTEM_ERROR_READ;
                }
                memcpy(buffer, tempBuffer, dataSize);
            }
            return FILE_SYSTEM_SUCCESS; // 成功
        } else {
            // 不正な値
            return FILE_SYSTEM_ERROR_READ;
        }
    }
}
