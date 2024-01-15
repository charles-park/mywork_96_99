//[*]--------------------------------------------------------[*]
#include <stdio.h>
#include <conio.h>
//[*]--------------------------------------------------------[*]
class CSam
{
	public:
		float	Aver;
		float	Spec;
		float	Max;
		float	Min;
		int 	id;

	public:
		CSam()					{	printf("Sam Initialize..\n");				}
		virtual~CSam()			{	printf("Sam unInitialize..\n");				}
		virtual void Sampling()	{	printf("Sam Sampling ...\n");  Store();		}

		virtual void Store();
		virtual void ReStore()	{	printf("Mpm Mem Read ...\n");				}

		virtual void Send()		{	ReStore(); printf("Mpm Pc Send  ...\n");	}
};
//[*]--------------------------------------------------------[*]
void CSam::Store()
{
	printf("Data Storage...\n");
}
//[*]--------------------------------------------------------[*]
class CSamWind : virtual public CSam
{
	public:
		CSamWind()		{	printf("Wind Initialize..\n");				}
		~CSamWind()		{	printf("Wind unInitialize..\n");			}
		void Sampling()	{   printf("Wind Sampling ...\n"); Store();		}
};

//[*]--------------------------------------------------------[*]
class CSamTemp : virtual public CSam
{
	public:
		CSamTemp()			{	printf("Temp Initialize..\n");				}
		~CSamTemp()			{	printf("Temp unInitialize..\n");			}
		void Sampling()		{   printf("Temp Sampling ...\n"); Store();		}
};
//[*]--------------------------------------------------------[*]
void main(void)
{
	CSam	*SamGroup[2];

	SamGroup[0] = new CSamWind;
	SamGroup[1] = new CSamTemp;

	int key = 0;

	while(1)	{
		key = 0;
		if(kbhit())	key = getch();
		if(key == 's')	{
			for(int i=0;i<2;i++)	SamGroup[i]->Sampling();
		}
		if(key == 'a')	for(int i=0;i<2;i++)	SamGroup[i]->Send();
		if(key == 'x')	break;
	}

	delete SamGroup[0];
	delete SamGroup[1];

}