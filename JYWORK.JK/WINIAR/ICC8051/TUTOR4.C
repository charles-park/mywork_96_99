
int ccount;

non_banked void loop10(void)
  {
    int loopc;
  
    for (loopc = 1; loopc < 10; loopc++);
  }

void printcnt(int ccnt)
  {
    printf("the count is now %d\n",ccnt);
  }

void main(void)     
  {
    int my_int = 0;
    int my_int2 = 0;
  
    ccount = 1;

    while (my_int < 100)
      {
	loop10();
	my_int++;
	printcnt(my_int);
      }
  }
