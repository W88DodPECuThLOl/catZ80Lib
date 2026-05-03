#include "x1_dma_textClearScreen40.h"
#include "x1_dma_fillVRAM.h"
#include "x1_dma_reset.h"

void
x1_dma_textClearScreen40()
{
    x1_dmaReset();
    x1_dmaFillVRAM(0x2000, 40*25, 0x00);
    x1_dmaFillVRAM(0x3000, 40*25, 0x20);
}
