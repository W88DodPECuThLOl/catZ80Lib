    .z80

    .module x1_crtc_setWidth40

    ; void x1_crtcSetWidth40()
    .globl _x1_crtcSetWidth40

    .area _CODE

; -------------------------------------------------------------------
; void x1_crtcSetWidth40();
_x1_crtcSetWidth40:
    ld      hl, #width40data
    ld      bc, #0x1801
    ld      a, (hl)
    inc     hl
crtcWidthloop:
        ; 書き込むレジスタ設定
        dec     c
        inc     b
        outi
        ; 値を書き込む
        inc     c
        inc     b
        outi
    dec     a
    jr      nz, crtcWidthloop
    ; 0x1Ax3 8255
    ld      bc, #0x1a03 + #0x100
    outi
    ; 0x1FDx 画面管理
    ld      bc, #0x1fd0 + #0x100
    outi
    ret

width40data:
    ; CRTCに設定する数
    .db #12
    ; R0 水平総文字数-1
    .db	#0, #55
    ; R1 水平表示文字数
    .db #1, #40
    ; R2 水平同期位置-1
    .db #2, #45
    ; R3 同期パルス幅
    .db #3, #52
    ; R4 垂直総文字数-1
    .db #4, #31
    ; R5 総ラスタ調整
    .db #5, #2
    ; R6 垂直表示文字数
    .db #6, #25
    ; R7 垂直同期位置-1 
    .db #7, #28
    ; R8 インタレース、スキュー
    .db #8, #0
    ; R9 最大ラスタアドレス
    .db #9, #7
    ; R12 スタートアドレス上位
    .db #12, #0
    ; R12 スタートアドレス下位
    .db #13, #0
    ; 0x1Ax3 8255 PC6のセット 40桁モード
    .db #0x0d
    ; 0x1FDx 画面管理
    .db #0x00 ; 低解像モニタ(200ライン)、25行
