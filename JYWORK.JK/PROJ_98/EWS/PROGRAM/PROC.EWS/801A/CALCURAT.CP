//[*]----------------------------------------------------------------------------------------------------------------
float che_cal_windNo(float humidity,int temperature)
{
	float tt = temperature - 0.4  *(temperature - 10)*(1 - humidity/100);
	return tt;
}
//[*]----------------------------------------------------------------------------------------------------------------
float che_cal_windYes(float windspd,float temperature)
{
	float tt = 33. - ((10.45 + 10 * (float)(sqrt(windspd)) - windspd) * (33. -temperature) / 20.04);
	return tt;
}
//[*]----------------------------------------------------------------------------------------------------------------
float bul_cal(float dewPoint, float temperature)
{
	double t, td, tf, tt, ttd, a, b, c, rh, di;
	t   	= (double)(temperature);
	td  	= (double)(dewPoint);
	tf  	= 9./5. * t  + 32;
	tt  	= t  + 273.15;
	ttd 	= td + 273.15;
	a   	= tt - ttd;
	b   	= tt * ttd;
	c   	= 4.25 * pow(10.,-4.);
	rh      = pow(10.,(-a / (b * c)));
	di      = tf - 0.55 * (1. - rh) * (tf - 58.);
	return (float)(di);
}
//[*]----------------------------------------------------------------------------------------------------------------
