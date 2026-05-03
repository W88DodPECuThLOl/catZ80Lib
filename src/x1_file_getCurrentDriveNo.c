#include "x1_file_getCurrentDriveNo.h"
#include "x1_fs_def.h"

u8
x1_fileGetCurrentDriveNo(u8* driveNo)
{
    if(gFileSystemContext.currentDriveNo >= 4) {
        return FILE_SYSTEM_ERROR_INVALID_DRIVE_NO;
    }
    *driveNo = gFileSystemContext.currentDriveNo;
    return FILE_SYSTEM_SUCCESS;
}
