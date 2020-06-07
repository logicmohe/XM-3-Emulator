StkTop equ #FFC0 ; TOS address
;
org #80
OEWord word #0001 ; Word to check
;
org #1000
Start ; Start of program
movls StkTop,R6 ; R6 (SP) <- #FFC0
;
movlz OEWord,R0 ; R0 <- addr of OEWord
ld R0,R0 ; R0 <- mem[R0 or OEWord's address]
;
; Check for odd or even
;
BL OE_Check ; Call OE_Check
CEX CS,#1,#0
BRA Odd
; Even code
Even bra Even
; Odd code
Odd bra Odd
;
; Check for LSbit set (odd) or clear (even)
; R0 contains word to check for odd or even
; Destructive test (using AND rather than BIT), use stack to save R0
; Return PSW.C: 0 (even) or 1 (odd)
;
OE_Check 
ST R0,-R6 ; Store R0 (stack)
and #1,R0 ; Mask LSbit
cmp #0,R0 ; R0 = #0?
CEX EQ,#1,#0
BRA OE_Check_Zero
SETCC C ; C <- 1 if odd
bra OE_Check_Done
OE_Check_Zero 
CLRCC C ; C <- 0 if even
OE_Check_Done 
LD R6+,R0 ; Restore R0 (stack)
MOV R5,R7 ; Return 
;
end Start
spl2 
SPL7 
SPL5 
MOV R5,R7
