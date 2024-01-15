#if __TID__ & 0x8000
#pragma function=intrinsic(0)
#endif
unsigned char _opc(unsigned char);

unsigned char _tbac(bit);

#if __TID__ & 0x8000
#pragma function=default
#endif


