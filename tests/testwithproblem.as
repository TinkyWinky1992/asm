MAIN: mov M1[r2][r7],LENGTH
 add r99 ; r99 is not a valid operand and add need two operands
LOOP: jmp END
 prn #-5
 sub r1, r4
 inc K

 mov M1[r3][r3],r3
 bne LOOP
     stop    ;here is a problem (END: label not exsits)
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22
M1: .mat [2][2] 1,2,3,4