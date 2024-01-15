a8051 d_iar51.asm,d_iar51,,s
icc8051 tiny51 -ms -P -e -r -g -z -L -q
icc8051 exmpl1 -ms -P -e -r -g -z -L -q
icc8051 exmpl2 -ms -P -e -r -g -z -L -q
icc8051 exmpl3 -ms -P -e -r -g -z -L -q
icc8051 exmpl4 -ms -P -e -r -g -z -L -q
icc8051 exmpl5 -ms -P -e -r -g -z -L -q
icc8051 exmpl6 -ms -P -e -r -g -z -L -q
xlink exmpl1 tiny51 d_iar51 -f tiny51 -o exmpl1 -l exmpl1.map -x
xlink exmpl2 tiny51 d_iar51 -f tiny51 -o exmpl2 -l exmpl2.map -x
xlink exmpl3 tiny51 d_iar51 -f tiny51 -o exmpl3 -l exmpl3.map -x
xlink exmpl4 tiny51 d_iar51 -f tiny51 -o exmpl4 -l exmpl4.map -x
xlink exmpl5 tiny51 d_iar51 -f tiny51 -o exmpl5 -l exmpl5.map -x
xlink exmpl6 tiny51 d_iar51 -f tiny51 -o exmpl6 -l exmpl6.map -x
