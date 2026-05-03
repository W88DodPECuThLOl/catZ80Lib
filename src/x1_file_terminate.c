#include "x1_file_terminate.h"
#include "x1_fs_def.h"

void
x1_fileTerminate()
{
    gFileSystemContext.readWriteBuffer = (u8*)0;
    gFileSystemContext.fatBuffer = (u8*)0;
    gFileSystemContext.currentDriveNo = 0;
    gFileSystemContext.driveContext[0].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    gFileSystemContext.driveContext[1].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    gFileSystemContext.driveContext[2].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    gFileSystemContext.driveContext[3].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
}
