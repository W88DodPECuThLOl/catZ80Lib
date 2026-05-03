#include "x1_file_setCurrentDriveNo.h"
#include "x1_fs_def.h"

u8
x1_fileSetCurrentDriveNo(const u8 driveNo)
{
    if(driveNo < 4) {
        gFileSystemContext.currentDriveNo = driveNo;
        return FILE_SYSTEM_SUCCESS;
    } else {
        return FILE_SYSTEM_ERROR_INVALID_DRIVE_NO;
    }
}
