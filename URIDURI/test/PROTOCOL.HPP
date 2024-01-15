#ifndef	__PROTOCOL_HPP__
#define	__PROTOCOL_HPP__
//[*]----------------------------------------------------------------------[*]
#include <string.h>
//[*]----------------------------------------------------------------------[*]
_CLASSDEF(protocol)
_CLASSDEF(protocol_grp)
//[*]----------------------------------------------------------------------[*]
class	_CLASSTYPE	protocol	{
	protected:
		char * protocol_q;
		int				sp, ep, revcnt, qsize, revflag;
		int 		  (*protocol_check_func)(protocol *p);
		int 		  (*protocol_catch_func)(protocol *p);

	public	 :
	   ~protocol();
		protocol();
		protocol(int size,int (*chk)(protocol *p),int (*cat)(protocol *p));
		void init_protocol(int size,int (*chk)(protocol *p),int (*cat)(protocol *p));

		int  get_flag (void)					 { return    revflag;		}
		int  get_sp   (void)					 { return    sp;			}
		int  get_ep   (void)					 { return    ep;			}
		int  get_cnt  (void)					 { return    revcnt;		}
		int  get_qsize(void)					 { return    qsize;			}

		void set_flag 	 (int status)			 { revflag = status;		}
		void set_sp   	 (int status)			 { sp      = status;		}
		void set_ep   	 (int status)			 { ep      = status;		}
		void set_cnt  	 (int status)			 { revcnt  = status;		}
		void set_chk_func(int (*f)(protocol *p)) { protocol_check_func = f; }
		void set_cat_func(int (*f)(protocol *p)) { protocol_catch_func = f; }

		int	 check_sp(int pos, char chk)
		{
			if(protocol_q[(sp + pos        ) % qsize] == chk) return 1;
			return 0;
		}

		int  check_ep(int pos, char chk)
		{
			if(protocol_q[(sp + qsize - pos) % qsize] == chk) return 1;
			return 0;
		}

		void set_qsize(int status);
		int	 check_protocol(void);
		int  catch_protocol(void);
		void protocol_Q_routine(char recive_data);
		char *get_qbuffer (int mode = 1);		 //	mode == 1 Q 	   Buffer
												 //	mode != 1 Sequence Buffer
		void clear_qbuffer(int mode = 1);		 //	mode == 1 is q postion clear
												 //	mode != 1 is q postion & buffer clear
};
//[*]----------------------------------------------------------------------[*]
class	_CLASSTYPE	protocol_grp	{
	protected	:
		int		  				  revport;
		int							count;
		int			(*get_data)(int port);

	public		:
		protocol     				* grp;
	public		:

	   ~protocol_grp();
		protocol_grp();
		protocol_grp(int port, int cnt, int (*get_data_func)(int port));

		void protocol_close(void);
		void protocol_event(void);
		int  get_port 	   (void)		{	return   revport;	}
		void set_port 	   (int port)	{	revport =   port;	}

};
//[*]----------------------------------------------------------------------[*]
#endif 	__PROTOCOL_HPP__
//[*]----------------------------------------------------------------------[*]
