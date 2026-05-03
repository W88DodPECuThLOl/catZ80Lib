    .z80

    .module x1_crtc_setPCG
	.allow_undocumented

	; void x1_crtcSetPCG(const u8* pcgDataAddress);
    .globl _x1_crtcSetPCG

	.area _CODE

; -------------------------------------------------------------------
; void x1_crtcSetPCG(const u8* pcgDataAddress);
_x1_crtcSetPCG:
	; PCG高速アクセス
	ld  bc, #0x1fd0
	in  a,(c)
	or  #0x20
	out (c),a

	; 漢字
	ld bc, #0x3FFF
	.db #0xED, #0x71 ; out (c),#0 ; Output 0 to port BC.
	; 属性
	ld b, #0x27		; bc: 0x27FF
	ld a, #0x20
	out (c),a

	; 設定する文字の数
	ld d,(hl)
	inc hl
loop0:
		; 設定する文字
		ld bc, #(0x37FF + 0x100)
		outi

		; pcg書き込む 
		; b
		ld bc, #(0x1500 + 0x100)
		call pcgWriteSub
		; r
		ld bc, #(0x1600 + 0x100)
		call pcgWriteSub
		; g
		ld bc, #(0x1700 + 0x100)
		call pcgWriteSub
	dec d
	jr nz,loop0

	; PCG通常アクセス
	ld bc, #0x1fd0
	in a,(c)
	and #0xdf
	out (c),a
	ret

pcgWriteSub:
	outi
	ld e, #7
loop1:
		inc c
		inc c
		inc	b
		outi
	dec e
	jr nz,loop1
	ret
