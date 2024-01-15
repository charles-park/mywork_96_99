    NAME    INITIAL_AND_POWER
; µa‹¡¬á“e ¡¡—e Ïa¡‹aœ‘µA ¢ ·¶´á´¡ Ði ¦¦…·±...
$reg8096.INC
    PUBLIC  CHIP_CONFIG
    PUBLIC  ASM_START
    PUBLIC  ITIME
    PUBLIC  IADC
    PUBLIC  IHSI
    PUBLIC  IHSO
    PUBLIC  IHSIO
    PUBLIC  ISOFT
    PUBLIC  ISERIAL
    PUBLIC  IEXT
    PUBLIC  IENABLE
    PUBLIC  IDISABLE

    EXTERN  main
    EXTERN  ?C_EXIT

CHIP_CONFIG EQU   0CDH ;11001101B <<= ¶aˆt·e Ða—a¶Á´áµA  x‰A ¹¡¸÷Ðiˆõ


        RSEG    CODE
ITIME:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_TIMER
        LCALL   interrupt_TIMER

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
IADC:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_AD
        LCALL   interrupt_AD

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
IHSI:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_HSI
        LCALL   interrupt_HSI

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
IHSO:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_HSO
        LCALL   interrupt_HSO

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
IHSIO:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_HSIO
        LCALL   interrupt_HSIO

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
ISOFT:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_STIMER
        LCALL   interrupt_STIMER

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
ISERIAL:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_SCON
        LCALL   interrupt_SCON

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; --
IEXT:
        PUSHF
        PUSH    R0
        PUSH    R2
        PUSH    R4
        PUSH    R6
        PUSH    R8

        EXTERN  interrupt_EINT
        LCALL   interrupt_EINT

        POP     R8
        POP     R6
        POP     R4
        POP     R2
        POP     R0
        POPF
        RET
; ==
IENABLE:
        EI
        RET
; ==
IDISABLE:
        DI
        RET
; ==
ASM_START:
        LD      SP,#100H

        EXTERN  ?SEG_INIT_L00
        LCALL   ?SEG_INIT_L00

        LCALL   main
        LJMP    ASM_START

        END
