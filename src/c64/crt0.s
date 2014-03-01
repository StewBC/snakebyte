;
; Startup code for cc65 (C64 version)
;

        .export         _exit
        .export         __STARTUP__ : absolute = 1      ; Mark as startup
        .import         initlib, donelib
        .import         zerobss
        .import         callmain
        .import         RESTOR, BSOUT, CLRCH
        .import         __RAM_START__, __RAM_SIZE__     ; Linker generated
		.import			__RODATA_LOAD__, __RODATA_RUN__, __RODATA_SIZE__	; Linker generated
		.import			__DATA_LOAD__, __DATA_RUN__, __DATA_SIZE__	; Linker generated
        .import         __STACKSIZE__                   ; Linker generated
        .importzp       ST

        .include        "zeropage.inc"
        .include        "c64.inc"


; ------------------------------------------------------------------------
; Startup code

.segment        "STARTUP"

Start:

; Save the zero page locations we need

        ldx     #zpspace-1
L1:     lda     sp,x
        sta     zpsave,x
        dex
        bpl     L1

; Switch to second charset

;        lda     #14
;        jsr     BSOUT

; Switch off the BASIC ROM

        lda     $01
        pha                     ; Remember the value
        and     #$F8
        ora     #$06            ; Enable kernal+I/O, disable basic
        sta     $01

; Clear the BSS data

        jsr     zerobss

		; Move DATA into place

			lda	#<__DATA_RUN__
			sta	ptr1
			lda	#>__DATA_RUN__
			sta	ptr1+1
			lda #<__DATA_LOAD__
			sta ptr2
			lda #>__DATA_LOAD__
			sta ptr2+1
			lda	#0
			tay

		; Move full pages

			ldx	#>__DATA_SIZE__
			beq	D3
		D2:	lda (ptr2),y
			sta	(ptr1),y
		   	iny
		   	bne	D2
			inc	ptr1+1
			inc ptr2+1
			dex
			bne	D2

		; Move remaining page (y is zero on entry)

		D3:	cpy	#<__DATA_SIZE__
			beq	D4
			lda (ptr2),y
			sta	(ptr1),y
			iny
			bne	D3
		D4:

		; Move RODATA into place

			lda	#<__RODATA_RUN__
			sta	ptr1
			lda	#>__RODATA_RUN__
			sta	ptr1+1
			lda #<__RODATA_LOAD__
			sta ptr2
			lda #>__RODATA_LOAD__
			sta ptr2+1
			lda	#0
			tay

		; Move full pages

			ldx #>__RODATA_SIZE__
			beq	RO3
		RO2:lda (ptr2),y
			sta	(ptr1),y
		   	iny
		   	bne	RO2
			inc	ptr1+1
			inc ptr2+1
			dex
			bne	RO2

		; Move remaining page (y is zero on entry)

		RO3:cpy	#<__RODATA_SIZE__
			beq	RO4
			lda (ptr2),y
			sta	(ptr1),y
			iny
			bne	RO3
		RO4:


; Save system settings and setup the stack

        pla
        sta     mmusave         ; Save the memory configuration

        tsx
        stx     spsave          ; Save the system stack ptr

        lda     #<(__RAM_START__ + __RAM_SIZE__ + __STACKSIZE__)
        sta     sp
        lda     #>(__RAM_START__ + __RAM_SIZE__ + __STACKSIZE__)
        sta     sp+1            ; Set argument stack ptr

; Call module constructors

        jsr     initlib

; Push arguments and call main

        jsr     callmain

; Back from main (This is also the _exit entry). Run module destructors

_exit:  pha                     ; Save the return code on stack
        jsr     donelib

; Copy back the zero page stuff

        ldx     #zpspace-1
L2:     lda     zpsave,x
        sta     sp,x
        dex
        bpl     L2

; Place the program return code into ST

        pla
        sta     ST

; Restore system stuff

        ldx     spsave
        txs                     ; Restore stack pointer
        ldx     mmusave
        stx     $01             ; Restore memory configuration

; Back to basic

        rts

; ------------------------------------------------------------------------
; Data

.segment        "ZPSAVE"

zpsave: .res    zpspace

.bss

spsave: .res    1
mmusave:.res    1
