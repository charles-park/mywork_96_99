        NAME    flprog

        ORG    $4000
        cmpi   #0,d0
        beq    flash_init
        cmpi   #1,d0
        beq    flash_block_programming
        cmpi   #2,d0
        beq    flash_programming
        cmpi   #3,d0
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
        cmpi    #1,d1
        beq     flash_programming_byte
        cmpi    #2,d1
        beq     flash_programming_word
        move.l  d2,(a0)  
        bra     flash_prgramming_end 
flash_programming_byte
        move.b  d2,(a0)  
        bra     flash_prgramming_end 
flash_programming_word
        move.w  d2,(a0)  
flash_prgramming_end 
        bgnd

*
* CODE FOR FLASH PROGRAMMING DONE
flash_done:
        bgnd 

        END


