; Assume Data, Alpha, and Beta are R0, R1, and R2, respectively
; IF Data = 8 THEN
org $2000
Start
cmp $8,R0
;
; Test the PSW bit for the NE condition (Z=0)
; If Z=0, then do the instruction following the CEX, otherwise skip
; There is no false-count, so execution would being immediately following
; the BRA:
;
cex ne,$1,$0
bra ElsePart ; Executed if NE condition is met
;
; Control passes here if NE condition not met (i.e., Data = 8)
;
movlz $1,R1
bra EndIf
;
; ELSE
; Control passes here is NE condition is met (i.e., Data != 8)
;
ElsePart
movlz $2,R1
;
EndIf
end Start