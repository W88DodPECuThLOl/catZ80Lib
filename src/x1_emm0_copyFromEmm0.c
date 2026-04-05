#include "x1_emm0_copyFromEMM0.h"
#include "x1_emm0_setEmm0Address.h"

static void
copyMainMem(u8* dst, u16 size) __naked
{
    (void)dst;
    (void)size;

    // hl : dst
    // de : size
    __asm

    ; コピー
    ld  bc,#0x0D03 ; EMM0
emmloopMainMem:
    in  a,(c)
    ld  (hl),a
    inc hl
    dec de
    ld  a,d
    or  a,e
    jr  nz, emmloopMainMem

    ret
    __endasm;
}

void
x1_copyFromEmm0(u8* dst, u32 emmAddress, u16 size)
{
    // 転送開始アドレス設定
    x1_setEmm0Address(emmAddress);
    // 転送
    copyMainMem(dst, size);
}
