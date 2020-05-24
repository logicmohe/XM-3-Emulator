;
; Sample XM2 file with legacy and emulated instructions
; Assemble this to find the unsupported instructions that need to be
; translated into XM3.
;
; The error message appears under the record in error.
;
; The assembler missed one of the errors - can you explain why?
;
; ECED 3403
; 5 May 2020
;
; All XM2 branching instructions (BEQ, BZ, BNE, BNZ, BGE, BLT, BC, BNC, and
; BN) must be replaced with corresponding CEX instruction.
;
; Subroutine call instructions (CALL, PULL, PUSH, and RET) must be replaced
; according to the following rules:
;
; CALL xxx -> bl xxx
; PULL xxx -> ld R6+,xxx
; PUSH xxx -> st xxx,-R6
; RET      -> mov R5,R7
;
; JUMP must be replaced with MOV arg,R7.  CLR
;
; All other instructions are a simple 1:1 conversion.  For example, CLC 
; becomes CLRCC C.
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
	CALL	OE_Check	; Call OE_Check
	BC	Odd
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
	PUSH	R0	; Store R0 (stack)
	and	#1,R0	; Mask LSbit
	cmp	#0,R0	; R0 = #0?
	BEQ	OE_Check_Zero
	SEC		; C <- 1 if odd
	bra	OE_Check_Done
OE_Check_Zero
	CLC		; C <- 0 if even
OE_Check_Done
	PULL	R0	; Restore R0 (stack)
	RET		; Return 
;
	end	Start
