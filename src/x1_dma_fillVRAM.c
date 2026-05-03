#include "x1_dma_fillVRAM.h"

// 参考）
// https://kyouichisato.blogspot.com/2014/09/bds-c-x1-turbo-dma.html

static u8 dmaCommandSetVRAMData = 0;
static char dmaCommandSetVRAM[14] = {
    0x7D,   /* 0111 1101    WR0 */
            /*       +----- 0:B->A 1:A->B */
    0x00,   /* PORT A アドレスL */
    0x00,   /* PORT A アドレスH */
    0x00,   /* ブロックレングスL */
    0x00,   /* ブロックレングスH */

    0x24,   /* 0010 0100    WR1 PORT A */
            /*   || +------ 0:メモリー 1:I/O */
            /*   ++-------- 00:-- 01:++ 10/11:固定 */

    0x18,   /* 0001 1000    WR2 PORT B */
            /*   || +------ 0:メモリー 1:I/O */
            /*   ++-------- 00:-- 01:++ 10/11:固定 */

    0xCD,   /* 1100 1101    WR4 */
            /*  ++--------- 00:バイト 01:コンティニュアス 10:バースト */
    0x00,   /* PORT B アドレスL */
    0x00,   /* PORT B アドレスH */

    0x9A,   /* 1001 1010    WR5 */
            /*    | +------ 0:READY L 1:READY H */
            /*    +-------- 0:CE 1:CE/WAIT */
    0xCF,   /*              WR6 LOAD */
    0xB3,   /*              WR6 FORCE READY */

    0x87    /* ENABLE DMA */
};

void
x1_dmaFillVRAM(const u16 vramAddress, u16 fillSize, const u8 fillValue)
{
    fillSize--;     /* DMAの都合により -1 */
    dmaCommandSetVRAMData = fillValue;

    dmaCommandSetVRAM[ 1]=(u16)&dmaCommandSetVRAMData;       /* PORT A アドレスL */
    dmaCommandSetVRAM[ 2]=(u16)&dmaCommandSetVRAMData >> 8;    /* PORT A アドレスH */
    dmaCommandSetVRAM[ 3]=fillSize;       /* ブロックレングスL */
    dmaCommandSetVRAM[ 4]=fillSize>>8;    /* ブロックレングスH */
    dmaCommandSetVRAM[ 8]=vramAddress;       /* PORT B アドレスL */
    dmaCommandSetVRAM[ 9]=vramAddress>>8;    /* PORT B アドレスH */
    __asm
    ld hl, #_dmaCommandSetVRAM
    ld bc, #(0x1f80 + 0x100)
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    inc b
    outi
    __endasm;
}
