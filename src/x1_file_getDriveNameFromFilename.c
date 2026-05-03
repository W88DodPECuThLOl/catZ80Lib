#include "x1_file_getDriveNameFromFilename.h"
#include "x1_fs_def.h"

u8
x1_fileGetDriveNameFromFilename(char* driveName, const u8 driveNameSize, const char* filename)
{
    char* dst = driveName;
    char* dstEnd = driveName + driveNameSize;
    const char* src = filename;
    while(*src != 0 && dst != dstEnd) {
        if(*src == ':') {
            if(dst != dstEnd) {
                *dst = 0;
            }
            return FILE_SYSTEM_SUCCESS;
        }
        *dst++ = *src++;
    }
    return FILE_SYSTEM_ERROR;
}
