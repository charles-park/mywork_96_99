
/* extended keywords */
#pragma language=extended

/*declaration of variables into different memory segments */
const float float1 = 5.0;		/* float constant in code memory */
code  float float2 = 6.0;		/* float constant in code memory */
idata char char1;			/* char in idata memory  */
int data int1;				/* int in data memory */
xdata char arr[] = "hello there!";	/* char array in xdata  */


/* 2-byte pointer to xdata placed into idata memory */
idata char xdata * xdata_pointer1;  

bit func (				/* function that returns a bit */
  bit b,				/* parameters: bit in bit memory */ 
  xdata char *gp,			/* generic pointer to xdata memory */
  data char xdata *xp			/* pointer to xdata in data memory */
         )
{
  idata int data * data_pointer;	/* pointer to data placed in idata */
					/* memory */
  gp = (char *) 0x000030;		/* set to 0030 in idata memory */
  gp = (char *) 0x022000;		/* set to 2000 in code memory */
  gp = (char *) 0x030020;		/* set to 0020 in pdata memory */
  xp = (char xdata *) 0x3000;		/* set to 3000 in xdata memory */
}

monitor char monfunction(int k);	/* a monitor function */

/* use pragmas to accomplish the same declarations */
/* turn off extended language (use only ANSI C keywords) */
#pragma language=default

/* declare variables in different memory segments */

const float float3 = 5.0;		/* float constant in code memory */

#pragma memory=code
float f2 = 6.0;				/* float constant in code memory */

#pragma memory=idata
char ch1;				/* char in idata memory  */

#pragma memory=xdata
char *gp1;				/* 3-byte generic ptr. in xdata */

#pragma function=monitor
char monfunc2(int i);			/* monitor function */

#pragma memory=dataseg(MYVECT)
char myca[100];				/* array in separate segment MYVECT */
					/* in xdata memory */

/* return to using memory model's default placement */
#pragma memory=default