#include "x1_dma_graphicsClearScreen.h"
#include "x1_dma_fillVRAM.h"
#include "x1_dma_reset.h"

void
x1_dmaGraphicsClearScreen()
{
    x1_dmaReset();
    x1_dmaFillVRAM(0x4000, 0xC000, 0x00);
}
