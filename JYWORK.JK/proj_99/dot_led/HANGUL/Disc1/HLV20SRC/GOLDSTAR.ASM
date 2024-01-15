;
;       GOLDSTAR Hangul Code Table
;
;       for Hurumi 2.0
;
;       written by Han Woo-jin
;

                        public _goldstartable
goldstartable_TEXT      segment byte public 'CODE'
_goldstartable          label byte

;table data

;chosung
           db  00h, 01h, 02h, 00h, 03h, 00h, 00h, 04h
           db  05h, 06h, 00h, 00h, 00h, 00h, 00h, 00h
           db  00h, 07h, 08h, 09h, 00h, 0ah, 0bh, 0ch
           db  0dh, 0eh, 0fh, 10h, 11h, 12h, 13h, 00h,00h
;jungsung
           db  00h, 00h, 01h, 02h, 03h, 04h, 05h, 06h
           db  00h, 00h, 07h, 08h, 09h, 0ah, 0bh, 0ch
           db  00h, 00h, 0dh, 0eh, 0fh, 10h, 11h, 12h
           db  00h, 00h, 13h, 14h, 15h, 00h, 00h, 00h,1fh
;jongsung
           db  00h, 01h, 02h, 03h, 04h, 05h, 06h, 07h
           db  00h, 08h, 09h, 0ah, 0bh, 0ch, 0dh, 0eh
           db  0fh, 10h, 11h, 00h, 12h, 13h, 14h, 15h
           db  16h, 00h, 17h, 18h, 19h, 1ah, 1bh, 00h,00h

goldstartable_TEXT      ends
                        end
