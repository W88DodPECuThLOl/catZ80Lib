#include "x1_file_enumerateInfomationBlock.h"
#include "x1_fdc_def.h"
#include "x1_disk_def.h"
#include "x1_fs_def.h"
#include "x1_disk_readRecord.h"

#define X1_FILE_DIR_ENTRY_END_MARK     (0xFF)
#define X1_FILE_DIR_ENTRY_DELETED_MARK (0x00)

u8
x1_fileEnumerateInfomationBlock(u8 driveNo, X1_FILE_CALLBACK_ENUMERATE_IB funcCallback, void* userData1, void* userData2, void* userData3)
{
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
            if(ib[0] == X1_FILE_DIR_ENTRY_END_MARK) {
                // 最後まで検索した
                return FILE_SYSTEM_SUCCESS;
            }
            if(ib[0] != X1_FILE_DIR_ENTRY_DELETED_MARK) {
                if(funcCallback(ib, userData1, userData2, userData3)) {
                    return FILE_SYSTEM_SUCCESS;
                }
            }
            ib += FILE_SYSTEM_IB_SIZE;
        }
    }
    return FILE_SYSTEM_SUCCESS;
}
