;
; Solution using XM3 to solve the odd/even problem
;
; Notice the difference between the legacy code, the hand translation, 
; and this version.
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
	bl	OE_Check	; Call OE_Check
	cex	cs,#1,#1
; Odd code
Odd	bra	Odd
; Even code
Even	bra	Even

;
; Check for LSbit set (odd) or clear (even)
; R0 contains word to check for odd or even
; Destructive test (using AND rather than BIT), use stack to save R0
; Return PSW.C: 0 (even) or 1 (odd)
;
OE_Check
	st	R0,-R6	; Store R6 (stack)
	and	#1,R0	; Mask LSbit
	cmp	#0,R0	; R0 = #0?
	cex	eq,#1,#1
	clrcc	c	; C <- 0 (even)
	setcc	c	; C <- 1 (odd)
	ld	R6+,R0	; Restore R6 (stack)
	mov	R5,R7	; R7 <- R5 (return address)
;
	end	Start
