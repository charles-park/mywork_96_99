rem Batch file to compile, assemble and link the example program.
c-96 example -ml -v0 -s10,64 -P -e -g -L -q -rn
a8096 exasm exasm
xlink -f example.xcl
rem All done!
rem You may now examine the files example.lst and example.map.
rem See the Tutorial section of the manual for more information.
