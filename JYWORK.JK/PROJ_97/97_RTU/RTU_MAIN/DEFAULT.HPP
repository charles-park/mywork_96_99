//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*]                        Milyang & Sanchung                           [*]
//[*]              97' AWS and AMOS RTU(Remote Terminal Unit)             [*]
//[*]                       factory setup program                         [*]
//[*]                Copyright Jinyang industrial co.,ltd                 [*]
//[*]                      Program by Jinmin - Mok                        [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
//[*] Sensor code list...												  [*]
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
#define	Wind_Direc		 0
#define	Wind_Spesd		 1
#define	Temper			 2
#define	Precipi			 3
#define	Prec_Detect		 4
#define	Pressure		 5
#define	Humidity		 6
#define	Dew_Temp		 7
#define	Sun_Rada		 8
#define	Sun_Detect		 9
#define	Evaporation		10
#define	Gnd_Humidit		11
#define	Snow			12
#define	Dewdrop			13
#define	Fog				14
#define	Cloud			15
#define	Visibility		16
#define	Bak_Lumi		17
#define	Sigma			18
#define	Sky_Emiss		19
#define	Gnd_Emiss		20
#define	Ph				21
#define	Conducte		22
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void factory_set(void);
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
void factory_set(void)
{
	idata word i;

	CFG_RAM(MPM_CFG);
	 for(i=0; i<200; i++)		output(ID_CODE +  i, 0xFD       );
	 output(ID_CODE     ,  0);	output(ID_CODE +  1, Wind_Direc );
	 output(ID_CODE +  2,  1);	output(ID_CODE +  3, Wind_Spesd );
	 output(ID_CODE +  4,  2);	output(ID_CODE +  5, Temper     );
	 output(ID_CODE +  6,  3);	output(ID_CODE +  7, Precipi    );
	 output(ID_CODE +  8,  4);	output(ID_CODE +  9, Humidity   );
	 output(ID_CODE + 10,  5);	output(ID_CODE + 11, Sun_Detect );
	 output(ID_CODE + 12,  6);	output(ID_CODE + 13, Temper     );
	 output(ID_CODE + 14,  7);	output(ID_CODE + 15, Humidity   );
	 output(ID_CODE + 16,  8);	output(ID_CODE + 17, Temper     );
	 output(ID_CODE + 18,  9);	output(ID_CODE + 19, Temper     );
	 output(ID_CODE + 20, 10);	output(ID_CODE + 21, Dewdrop    );
	RAM_CS();
	i_data.sensor_su  =    11;	mpm_cfg_save(SENSOR_SU );
	i_data.station_id =     1;	mpm_cfg_save(STATION_ID);
	i_data.password   = SUPER;	mpm_cfg_save(PASSWORD  );
	i_data.call_time  =     0;	mpm_cfg_save(CALL_TIME );
	i_data.save_time  =   600;	mpm_cfg_save(SAVE_TIME );
	i_data.day_cnt    =     0;	mpm_cfg_save(DAY_CNT   );

	sport[COM1].baud     = BPS9600;
	sport[COM1].data_bit = NC;
	sport[COM1].stop_bit = NC;
	sport[COM1].parity   = NC;
	sport[COM2].baud     = BPS2400;
	sport[COM2].data_bit = DATA_BIT_8;
	sport[COM2].stop_bit = STOP_BIT_1;
	sport[COM2].parity   = PARITY_NO;
	sport[COM3].baud     = BPS2400;
	sport[COM3].data_bit = DATA_BIT_8;
	sport[COM3].stop_bit = STOP_BIT_1;
	sport[COM3].parity   = PARITY_NO;
	sport_cfg_save(COM1);
	sport_cfg_save(COM2);
	sport_cfg_save(COM3);
}
//[*]ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ[*]
