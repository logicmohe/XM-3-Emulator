;
; Sample XM2 file with legacy and emulated instructions replaced with
; their XM3 equivalents.
;
; This was a hand translation.  Paraphrasing Babbage, "I wish to God these 
; instructions had been translated by a preprocessor".
;
; ECED 3403
; 5 May 2020
;
; **********************************************
; 
; The following code fragment determines if a word (in R0) is odd or even.
;
StkTop	equ	#FFC0		; TOS address
;
	org	#80
OEWord	word	#0001		; Word to check
;
	org	#1000
Start				; Start of program
	movls	StkTop,R6	; R6 (SP) <- #FFC0
;
	movlz	OEWord,R0	; R0 <- addr of OEWord
	ld	R0,R0		; R0 <- mem[R0 or OEWord's address]
;
; Check for odd or even
;
;	** CALL	OE_Check	; Call OE_Check
	bl	OE_Check	; Call OE_Check
;
; 	** BC	Odd
	cex	cs,#1,#0
	bra	Odd
; Even code
Even	bra	Even
; Odd code
Odd	bra	Odd
;
; Check for LSbit set (odd) or clear (even)
; R0 contains word to check for odd or even
; Destructive test (using AND rather than BIT), use stack to save R0
; Return PSW.C: 0 (even) or 1 (odd)
;
OE_Check
;	** PUSH	R0	; Store R0 (stack)
	st	R0,-R6
;
	and	#1,R0	; Mask LSbit
	cmp	#0,R0	; R0 = #0?
;
;	** BEQ	OE_Check_Zero
	cex	eq,#1,#0
	bra	OE_Check_Zero
;
;	** SEC		; C <- 1 if odd
	setcc	c
	bra	OE_Check_Done
OE_Check_Zero
;
;	** CLC		; C <- 0 if even
	clrcc	c
OE_Check_Done
;
;	** PULL	R0	; Restore R0 (stack)
	ld	R6+,R0
;
;	** RET		; Return 
	mov	R5,R7
;
	end	Start
