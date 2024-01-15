        NAME    flprog

        ORG    $4000
        cpd    #0
        beq    flash_init
        cpd    #1
        beq    flash_block_programming
        cpd    #2
        beq    flash_programming
        cpd    #3
        beq    flash_done
        bgnd 

* 
*  CODE FOR  FLASH PROGRAMMING INITIALIZATION
flash_init
        bgnd 

*
*  CODE FOR FLASH BLOCK PROGRAMMING 
flash_block_programming
        bgnd

*
* CODE FOR PROGRAMMING 1 OR 2 BYTE
flash_programming
        xgde
        cpy    #1
        beq    flash_programming_1byte
        std    0,x
        bra    flash_prgramming_end 
flash_programming_1byte
        stab   0,x
flash_prgramming_end 
        bgnd

*
* CODE FOR FLASH PROGRAMMING DONE
flash_done:
        bgnd 

        END


