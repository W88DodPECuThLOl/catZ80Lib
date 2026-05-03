#include "x1_fdc_readData.h"
#include "x1_fdc_def.h"
#include "x1_disk_waitForNotBusy.h"

const u8
x1_fdcReadData(u8* data, const u8 sectorNo) __naked
{
    (void)data;
    (void)sectorNo;

    __asm
    pop af
    pop de
    push de
    push af

    ; セクターを設定
    push hl
        ld   bc, #FDC_PORT_SCR
        out  (c), e
        ld   b, #0x20 ; 少し待つらしい
wait0:  djnz wait0
        call _x1_diskWaitForNotBusy
    pop hl

    ld de, #0xf8fb
    ld bc, #FDC_PORT_CR
    ld a, #FDC_COMMAND_READ_DATA
    di
    out (c),a
    call ShortWait ; 少し待つらしい
readLoop:
    ; データ待ち
    in a,(c)
    rrca
    jr nc,readEnd
    rrca
    jr nc,readLoop

    ; 1バイト読み込む
    ld  c,e
    in  a,(c)
    ld  (hl),a
    inc hl
    ld  c,d
    jp  readLoop

    ; 少し待つ
ShortWait:
    ld a, #7
ShortWaitLoop:
    dec a
    jr nz,ShortWaitLoop
    ret

readEnd:
    ei
    rlca

    pop	hl
    inc	sp
    jp	(hl)
    __endasm;
}
