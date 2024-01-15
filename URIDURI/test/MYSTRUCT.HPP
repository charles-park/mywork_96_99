//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#ifndef	__MYSTRUCT_HPP__
#define	__MYSTRUCT_HPP__
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct rtu_recv__t	{
	unsigned long 	header;
	unsigned char	year  ;
	unsigned char	month ;
	unsigned char	day   ;
	unsigned char	hour  ;
	unsigned char	min   ;
	unsigned char   sec   ;
	unsigned char	pos   ;
	unsigned int	stid  ;
	unsigned int	preiod;
	unsigned char	sen_st;
	unsigned char	sen_cd;
	unsigned char	sen_id;
	float			aver  ;
	float			spec  ;
	float			mini  ;
	float			maxi  ;
	unsigned char	xor   ;
	unsigned char	add   ;
	unsigned long 	tail  ;
}	rtu_recv_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct send_pc__t	{
	unsigned char	header;
	unsigned char	cmd   ;
	unsigned int	tail  ;
}	send_pc_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct cmd__t	{
	unsigned long	header;
	unsigned char	year  ;
	unsigned char	month ;
	unsigned char 	day   ;
	unsigned char	hour  ;
	unsigned char	min   ;
	unsigned char 	sec   ;
	unsigned char	cmd   ;
	unsigned int	value ;
	unsigned char	xor   ;
	unsigned char   add   ;
	unsigned long	tail  ;
}	cmd_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	status_call__t	{
	unsigned long	header;
	unsigned char	year  ;
	unsigned char	month ;
	unsigned char 	day   ;
	unsigned char	hour  ;
	unsigned char	min   ;
	unsigned char 	sec   ;
	unsigned char	rtuid ;
	unsigned int	jijum ;
	unsigned char	bdid  ;
	unsigned int	cmd   ;
	unsigned char	p12   ;
	unsigned char	p5    ;
	unsigned char   m12   ;
	unsigned char	bd_sta[8];
	unsigned char	sen_sta[8];
	unsigned int	cnd_bd;
	unsigned char	xor   ;
	unsigned char	add   ;
	unsigned long	tail  ;
}	status_call_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
typedef struct	volts_call__t	{
	unsigned long	header;
	unsigned char	year  ;
	unsigned char	month ;
	unsigned char 	day   ;
	unsigned char	hour  ;
	unsigned char	min   ;
	unsigned char 	sec   ;
	unsigned char	rtuid ;
	unsigned int	jijum ;
	unsigned char	bdid  ;
	unsigned int	cmd   ;
	float 			p12   ;
	float 			p5    ;
	float 			m12   ;
	unsigned char	xor   ;
	unsigned char	add   ;
	unsigned long	tail  ;
}	volts_call_t;
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]
#endif	__MYSTRUCT_HPP__
//[*]컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴[*]












