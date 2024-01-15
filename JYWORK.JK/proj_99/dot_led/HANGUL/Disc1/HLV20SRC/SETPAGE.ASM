;
;
;
;       Hangul Library Ver 2.0 Paging Routine Init Module
;
;                               written by Han Woo-jin
;

.model large

                public _hgSetPageInit
                public _setpage

_TEXT           segment byte public 'CODE'
                assume cs:_TEXT

fpointer        db      4 dup(?)                ; save function pointer
                                                ; in code segment
_hgSetPageInit  proc    far
                ARG     rtoff:word,rtseg:word

                push bp
                mov bp,sp

                mov dx,rtseg            ; fetch segment
                mov ax,rtoff            ; fetch offset

                mov cs:word ptr fpointer+2,dx    ; save segment
                mov cs:word ptr fpointer,ax      ; save offset

                pop bp
                retf
_hgSetPageInit  endp

_setpage        proc    far
                push bp
                mov bp,sp

                call dword ptr cs:[fpointer]    ; call routine

                pop bp
                retf
_setpage        endp

_TEXT           ends
                end
