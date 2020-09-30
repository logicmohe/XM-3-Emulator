; Assume Data, Alpha, and Beta are R0, R1, and R2, respectively
; IF Data = 8 THEN
org #1000
Start
cmp $8,R0
;
; Test the PSW bit for the EQ condition (Z=1)
; If Z=1, then do the instruction following the CEX, otherwise skip
;
cex eq,$1,$1
;
Movlz $1,R1 ; Executed if EQ, skipped if NE
;
movlz $2,R1 ; Skipped if EQ, executed if NE
;
movlz $1,R2
end Start