//[*]----------------------------------------------------------------------[*]
#include "protocol.hpp"
//[*]----------------------------------------------------------------------[*]
protocol::~protocol()
{
	if(protocol_q)		delete [] protocol_q;
}
//[*]----------------------------------------------------------------------[*]
protocol::protocol()
{
	protocol_q 			= NULL;
	protocol_check_func = NULL;
	protocol_catch_func = NULL;
	sp = ep = revcnt = qsize = revflag = 0;
}
//[*]----------------------------------------------------------------------[*]
 protocol::protocol(int size,int (*chk)(protocol *p),int (*cat)(protocol *p))
{
	init_protocol(size, chk, cat);
}
//[*]----------------------------------------------------------------------[*]
void protocol::init_protocol(int size,int (*chk)(protocol *p),int (*cat)(protocol *p))
{
	qsize   = size;
	sp = ep = revcnt = revflag = 0;
	protocol_check_func = chk;
	protocol_catch_func = cat;
	protocol_q = new char[qsize];
}
//[*]----------------------------------------------------------------------[*]
void protocol::set_qsize(int status)
{
	if(qsize != status)	{
		qsize = status;
		if(protocol_q)	 delete [] protocol_q;
		protocol_q = new char[qsize];
	}
}
//[*]----------------------------------------------------------------------[*]
char *protocol::get_qbuffer(int mode /* = 1 */)
	//	mode == 1 Q 	   Buffer
{   //	mode != 1 Sequence Buffer
	if(mode != 1)	{
		char  *temp = new char[qsize];
		memcpy(temp, protocol_q, qsize);
		for(int i=0; i<qsize; i++)	{
			protocol_q[i] = temp[(sp+i) % qsize];
		}
		sp = 0;
		ep = qsize-1;
		delete [] temp;
	}
	return	protocol_q;
}
//[*]----------------------------------------------------------------------[*]
void protocol::clear_qbuffer(int mode /* = 1 */)
	//	mode == 1 is q postion clear
{	//	mode != 1 is q postion & buffer clear
	if(mode != 1)	memset(protocol_q, 0x00, qsize);
	sp = ep = revcnt = 0;
}
//[*]----------------------------------------------------------------------[*]
int	 protocol::check_protocol(void)
{
	if(protocol_check_func)	return protocol_check_func(this);
	return 0;
}
//[*]----------------------------------------------------------------------[*]
int  protocol::catch_protocol(void)
{
	if(protocol_catch_func)	return protocol_catch_func(this);
	return 0;
}
//[*]----------------------------------------------------------------------[*]
void protocol::protocol_Q_routine(char recive_data)
{
	ep %= qsize;
	if(sp == ep)	{	sp++; sp %= qsize;	}
	protocol_q[ep++] = recive_data;
}
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
//[*]--- PROTOCOL GROUP                                                 ---[*]
//[*]----------------------------------------------------------------------[*]
//[*]----------------------------------------------------------------------[*]
protocol_grp::protocol_grp()
{
	count    = 0;
	revport  = 0;
	get_data = NULL;
	grp      = NULL;
}
//[*]----------------------------------------------------------------------[*]
protocol_grp::~protocol_grp()
{
	protocol_close();
}
//[*]----------------------------------------------------------------------[*]
void protocol_grp::protocol_close(void)
{
	if(grp)	delete [] grp;
}
//[*]----------------------------------------------------------------------[*]
protocol_grp::protocol_grp(int port, int cnt, int (*get_data_func)(int port))
{
	count    = cnt;
	revport  = port;
	get_data = get_data_func;
	grp      = new protocol[count];
}
//[*]----------------------------------------------------------------------[*]
void protocol_grp::protocol_event(void)
{
	int	recive_data = get_data(revport);
	if (recive_data == -1)		 return;

	for(int i=0;i<count;i++)	{
		if(grp[i].get_flag())	{
			grp[i].protocol_Q_routine(recive_data);
			if(grp[i].check_protocol())	{
				if(grp[i].catch_protocol())	{
				   grp[i].set_flag(0);
				}
			}
		}
	}
}
//[*]----------------------------------------------------------------------[*]
