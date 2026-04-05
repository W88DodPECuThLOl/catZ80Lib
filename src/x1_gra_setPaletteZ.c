#include "x1_gra_setPaletteZ.h"
#include "z80_outp.h"

__sfr __banked __at(0x1FC5) PLTPT;

void
x1_setPaletteZ(u16 index, u16 rgb444)
{
	u8 palB =  rgb444       & 0xF;
	u8 palR = (rgb444 >> 4) & 0xF;
	u8 palG = (rgb444 >> 8) & 0xF;

	u8  indexL4 = (index & 0xF) << 4;
	u16 indexU4 = (index >> 4) & 0xFF;

	PLTPT  = 0x80; // アクセス有効、書き込みモード
	outp(0x1000 | indexU4, indexL4 | palB);
	outp(0x1100 | indexU4, indexL4 | palR);
	outp(0x1200 | indexU4, indexL4 | palG);
	PLTPT  = 0x00; // アクセス無効
}
