#include "x1_file_getFileInfomationBlock.h"
#include "x1_fs_def.h"

u8
x1_fileGetFileSize(const char* filename, u32* fileSize)
{
    u8 infomationBlock[FILE_SYSTEM_IB_SIZE];
    u8 rc = x1_fileGetFileInfomationBlock(filename, infomationBlock);
    if(rc != FILE_SYSTEM_SUCCESS) {
        *fileSize = 0;
        return rc;
    }

    *fileSize = x1_fileGetInfomationBlockFileSize(infomationBlock);
    return FILE_SYSTEM_SUCCESS;
}
