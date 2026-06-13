#include "x1_file_getFileInfomationBlock.h"
#include "x1_fs_def.h"

u8
x1_fileNormalizeFileName(char* normalizedFilename, const u8* filename)
{
    char* dst = normalizedFilename;
    char* dstEnd = dst + FILE_SYSTEM_FILE_NAME_LENGTH + FILE_SYSTEM_FILE_EXTENTION_LENGTH;
    while(dst != dstEnd) {
        *dst++ = 0x20;
    }
    *dst = 0;

    // ドライブ名を飛ばす
    const char* src = filename;
    while(*src != 0 && *src != ':') { src++; }
    if(*src == 0) {
        // ドライブ名が無かった
        src = filename;
    } else {
        src++;
    }
    // ファイル名をコピー
    dst = normalizedFilename;
    dstEnd = dst + FILE_SYSTEM_FILE_NAME_LENGTH;
    while(*src != 0 && *src != '.' && dst != dstEnd) {
        *dst++ = *src++;
    }
    if(*src == '.') {
        src++;
    } else if(*src == 0) {
        // ファイル名だけだった
        return FILE_SYSTEM_SUCCESS;
    } else if(dst == dstEnd) {
        return FILE_SYSTEM_ERROR_BAD_FILE_NAME;
    }
    // 拡張子をコピー
    dst = normalizedFilename + FILE_SYSTEM_FILE_NAME_LENGTH;
    dstEnd = dst + FILE_SYSTEM_FILE_EXTENTION_LENGTH;
    while(*src != 0 && dst != dstEnd) {
        *dst++ = *src++;
    }
    if(*src != 0) {
        return FILE_SYSTEM_ERROR_BAD_FILE_NAME;
    }
    return FILE_SYSTEM_SUCCESS;
}
