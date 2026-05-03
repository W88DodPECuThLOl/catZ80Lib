    .z80

    .module x1_dma_reset
	.allow_undocumented

	; void x1_dmaReset();
    .globl _x1_dmaReset

	.area _CODE

; -------------------------------------------------------------------
; void x1_dmaReset();
_x1_dmaReset:
    ld bc, #0x1f80
    ld a, #0xc3
    out (c), a
    out (c), a
    out (c), a
    out (c), a
    out (c), a
    out (c), a
    ret
