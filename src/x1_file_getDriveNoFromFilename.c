#include "x1_file_getDriveNoFromFilename.h"
#include "x1_file_getDriveNameFromFilename.h"
#include "x1_fs_def.h"

u8
x1_fileGetDriveNoFromFilename(u8* driveNo, const u8 defaultDriveNo, const char* filename)
{
    char driveName[17];
    if(x1_fileGetDriveNameFromFilename(driveName, 17, filename) == FILE_SYSTEM_SUCCESS) {
        if(65 <= driveName[0] && driveName[0] <= 68 && driveName[1] == 0) {
            // A: ～ D:
            // 0  ～ 3
            *driveNo = driveName[0] - 65;
            return FILE_SYSTEM_SUCCESS;
        } else if(97 <= driveName[0] && driveName[0] <= 100 && driveName[1] == 0) {
            // a: ～ d:
            // 0  ～ 3
            *driveNo = driveName[0] - 97;
            return FILE_SYSTEM_SUCCESS;
        }
        return FILE_SYSTEM_ERROR_INVALID_DRIVE_NAME;
    }
    if(defaultDriveNo >= 4) {
        return FILE_SYSTEM_ERROR_INVALID_DRIVE_NO;
    }
    *driveNo = defaultDriveNo;
    return FILE_SYSTEM_SUCCESS;
}
