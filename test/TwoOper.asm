org #1000
Start
;
; Example 1: R0 <- #1 – 0
; Result:
; R0=#0001
; C=1 (borrow not required)
; N=0 (non-negative result)
;
movlz #1,R0
sub #0,R0 ; R0 <- R0 - 0
;
; Example 2: R0 <- #1 - #2
; Result:
; R0=#FFFF ($-1)
; C=0 (borrow required)
; N=1 (negative result)
;
movlz #1,R1
sub #2,R1 ; R0 <- R0 - 2
;
; Example 3: R0 <- #1 + ($-1)
; Result:
; R0=#0002 ($2)
; C=0 (borrow required)
; N=1 (non-negative result)
;
movlz #1,R2
add $-1,R2 ; R2 <- R2 + (-1)
;
; Example 4: R3 <- #1 - #1
; Result:
; R03=#0000 ($0)
; C=1 (borrow not required)
; N=0 (negative result)
; Z=1 (zero result)
;
movlz #1,R3
addc #1,R0 ; R3 <- R3 + 1
end Start