    .z80

    .module asm_inp

	; u8 inp(u16 portAddress);
    .globl inp
    .globl _inp

	.area _CODE

; -------------------------------------------------------------------
; u8 inp(u16 portAddress);
; @param    hl  portAddress
; @return   a
inp:
_inp:
    ld b, h
    ld c, l
    in a, (c)
    ret
