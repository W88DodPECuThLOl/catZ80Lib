#include "x1_file_initialize.h"
#include "x1_fs_def.h"

FileSystemContext gFileSystemContext;

u8
x1_fileInitialize(u8* fatBuffer, u8* readWriteBuffer)
{
    if(fatBuffer == (u8*)0 || readWriteBuffer == (u8*)0) {
        return FILE_SYSTEM_ERROR;
    }
    gFileSystemContext.readWriteBuffer = readWriteBuffer;
    gFileSystemContext.fatBuffer = fatBuffer;
    gFileSystemContext.currentDriveNo = 0;
    gFileSystemContext.driveContext[0].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    gFileSystemContext.driveContext[1].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    gFileSystemContext.driveContext[2].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    gFileSystemContext.driveContext[3].driveType = FILE_SYSTEM_DRIVE_TYPE_2D;
    return FILE_SYSTEM_SUCCESS;
}
