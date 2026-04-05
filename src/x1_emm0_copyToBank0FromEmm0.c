#include "x1_emm0_copyToBank0FromEmm0.h"
#include "x1_emm0_setEmm0Address.h"

static void
copyBank0(u8* dst, u16 size) __naked
{
    (void)dst;
    (void)size;

    // hl : dst
    // de : size
    __asm
    ; バンク0へ切り替え
    ld   bc,#0x0B00
    di
    out  (c),c

    ; コピー
    ld  bc,#0x0D03 ; EMM0
emmloop0:
    in  a,(c)
    ld  (hl),a
    inc hl
    dec de
    ld  a,d
    or  a,e
    jr  nz, emmloop0

    ; メインメモリに切り替え
    ld   a, #0x10
    ld   bc,#0x0B00
    out  (c),a

    ei
    ret
    __endasm;
}

void
x1_copyToBank0FromEmm0(u8* dst, u32 emmAddress, u16 size)
{
    // 転送開始アドレス設定
    x1_setEmm0Address(emmAddress);
    // 転送
    copyBank0(dst, size);
}
