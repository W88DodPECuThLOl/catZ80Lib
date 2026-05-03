    .z80

    .module x1_crtc_waitVBlank

	; void x1_crtcWaitVBlank();
    .globl _x1_crtcWaitVBlank

	.area _CODE

; -------------------------------------------------------------------
; void x1_crtcWaitVBlank();
_x1_crtcWaitVBlank:
	ld	bc,#0x1a01
loop1:
	in	a,(c)
	jp	p,loop1
loop2:
	in	a,(c)
	jp	m,loop2
	ret
