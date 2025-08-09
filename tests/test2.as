.entry MAIN
.extern EXT_LABEL

MAIN:   mov  #5, r1
        add  EXT_LABEL, r1
        prn  r1
        stop

DATA1:  .data  7, -3, 15
STR1:   .string "hi"