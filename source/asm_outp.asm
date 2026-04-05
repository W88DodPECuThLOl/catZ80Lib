    .z80

    .module asm_outp

	; void outp(u16 portAddress, u8 value);
    .globl _outp

	.area _CODE

; -------------------------------------------------------------------
; void outp(u16 portAddress, u8 value);
; @param    hl  portAddress
; @param    sp  value
_outp:
    pop af
    pop de
    push de
    push af
        ld  c, l
        ld  b, h
        out (c), e
	pop	hl
	inc	sp
	jp	(hl)
