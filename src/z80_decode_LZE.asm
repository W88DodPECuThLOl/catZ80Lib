;//     LZE DECODE ROUTINE
;//     Original Copyright (C)1995,2008 GORRY.
;//     Porting for Z80 by Kei Moroboshi 2020/Apr
;       .Z80
;       PUBLIC	DECODE_LZE
;==============================================================================
;LZE data decoder
;Input:
;       HL      LZE encoded data address(*)
;       DE      extracting address
;(*)lze.exe add 4 byte header as extracted size(big-endian).
;Therefore, you must set HL as LZE data top + 4(skip the header).
;Broken:
;       AF,BC,DE,HL,AF',BC',DE',HL',IY
;Example:
;       LD      HL,LZEDATA+4
;       LD      DE,EXTRACT_AREA
;       CALL    DECODE_LZE
;==============================================================================

        .module z80_decode_LZE
        .globl  _z80_decodeLZE
        .area   _CODE
; for SDCC Entry
; void z80_decodeLZE(const u8* encodedDataAddress, u8* extractingAddress);
_z80_decodeLZE:
        PUSH    IY
        EXX
        PUSH    HL
        PUSH    DE
        PUSH    BC
        EXX
        INC     HL
        INC     HL
        INC     HL
        INC     HL
        CALL    DECODE_LZE
        EXX
        POP     BC
        POP     DE
        POP     HL
        EXX
        POP     IY
        RET

DECODE_LZE:                     ;ENTRY
        LD      IY,#MAINLP      ;[[[or remove]]]
        PUSH    DE
        EXX
        POP     DE
        LD      BC,#0           ;CopyCount=0
        EXX
        LD      D,#8

        LD      B,#1            ;BitCount=1, first

STORE:  LD      A,(HL)          ;*(DECODEPtr++) = *(LZEPtr++);
        INC     HL
        EXX
        LD      (DE),A          ;DE'=DECODEPtr
        INC     DE
        EXX

MAINLP:
        DJNZ    SKIP            ;BitTest() {
        LD      C,(HL)          ;
        INC     HL
        LD      B,D             ;8
SKIP:	SLA     C               ;}

        JR      C,STORE         ;1:Store
                                ;0x:Copy
;Copy Byte Distance or Word Distance
        DJNZ    SKIP0           ;BitTest() {
        LD      C,(HL)          ;
        INC     HL
        LD      B,D             ;8
SKIP0:	SLA     C               ;}

        JR      C,COPYWD        ;01:Copy with Word Distance

                                ;00:Copy with Byte Distance
COPYBD:
        DJNZ    SKIP00          ;BitTest() {
        LD      C,(HL)          ;
        INC     HL
        LD      B,D             ;8
SKIP00:	SLA     C               ;}

        EXX                     ;BC:CopyCount is already 0;
        RL      C               ;adc    CopyCount,CopyCount
        EXX

        DJNZ    SKIP01          ;BitTest() {
        LD      C,(HL)          ;
        INC     HL
        LD      B,D             ;8
SKIP01:	SLA     C               ;}

        LD      A,(HL)          ;LoadDistanceB();
        INC     HL
        EXX
        LD      L,A             ;Distance = -256+((int)*(LZEPtr++));
        LD      H,#0xFF
        RL      C               ;adc    CopyCount,CopyCount
;
CPCNTP2:                        ;CopyCount ++;
        INC     C               ;inc    CopyCount
CPCNTP1:
        INC     BC              ;inc    CopyCount
                                ;Copy() {
        ADD     HL,DE           ;       HL':CopyPtr=HL':Distance + DE':DECODEPtr
        LDIR                    ;}      //BC':CopyCount would be 0
        EXX
        JP      (IY)            ;[[[JP/JR       MAINLP]]]

;COPY with Word Distance
COPYWD:
        LD      A,(HL)          ;LoadDistanceW() {
        INC     HL
        EX      AF,AF'
        LD      A,(HL)
        INC     HL
        EXX
        LD      L,A             ;L=DistanceL
        EX      AF,AF'          ;A=DistanceH
                                ;mov    CopyCount,Distance
        RRA                     ;shr    Distance,1
        RR      L
        RRA                     ;shr    Distance,1
        RR      L
        RRA                     ;shr    Distance,1
        RR      L
        OR      #0xE0           ;or     DistanceH,0e0h
        LD      H,A             ;HL=(Distance>>3) | 0xE000;

        EX      AF,AF'          ;if ( (CopyCount &= 7) != 0 ) {
        AND     #7              ;  CopyCount += 2; Copy();
        LD      C,A
        JP      NZ,CPCNTP2      ;[[[ or JR ]]]
        EXX                     ;}else { // if CopyCount == 0
        LD      A,(HL)          ;  LoadCopyCount() {
        INC     HL
        EXX
        LD      C,A             ; } C'=CopyCountL
        OR      A               ;if ( CopyCount==0 ) goto DECODE_END
        JP      NZ,CPCNTP1      ;[[[ or JR ]]]
                                ;else goto CPCNTP1
DECODE_END:
        RET


