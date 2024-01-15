;
;       SANGYONG Hangul Code Table
;
;       for Hurumi 2.0
;
;       written by Han Woo-jin
;

                        public _sangyongtable
sangyongtable_TEXT      segment byte public 'CODE'
_sangyongtable          label byte

;table data

;chosung
           db  00h, 00h, 01h, 02h, 03h, 04h, 05h, 06h
           db  07h, 08h, 09h, 0Ah, 0Bh, 0Ch, 0Dh, 0Eh
           db  0Fh, 10h, 11h, 12h, 13h, 00h, 00h, 00h
           db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h,01h
;jungsung
           db  00h, 00h, 00h, 01h, 02h, 03h, 04h, 05h
           db  00h, 00h, 06h, 07h, 08h, 09h, 0Ah, 0Bh
           db  00h, 00h, 0Ch, 0Dh, 0Eh, 0Fh, 10h, 11h
           db  00h, 00h, 12h, 13h, 14h, 15h, 00h, 00h,02h
;jongsung
           db  00h, 00h, 01h, 02h, 03h, 04h, 05h, 06h
           db  07h, 08h, 09h, 0Ah, 0Bh, 0Ch, 0Dh, 0Eh
           db  0Fh, 10h, 00h, 11h, 12h, 13h, 14h, 15h
           db  16h, 17h, 18h, 19h, 1Ah, 1Bh, 00h, 00h,01h

sangyongtable_TEXT      ends
                        end


