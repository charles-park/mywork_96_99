#include <stdio.h>
#include <conio.h>

#include "serial.hpp"
#include "protocol.hpp"

void Hook_Com(char data)
{
	printf("%02X", data);
}

protocol_grp   *protocol;

int  check_func(protocol *p)
{
	char *buffer = p->get_qbuffer();
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	if(buffer[(p->get_sp() + 0) % get_size()] != 0xff)	return 0
	return 1;
}

int  catch_func(protocol *p)
{
	A a;
	char buffer[sizeof(A)];
	int  buffer[10];
	memcpy(buffer, &a, sizeof(A));


	clear_qbuffer()
	return 0;
}

void main()
{
	serial_init();

	com_open(COM1,BAUD(19200), DATABIT8,STOPBIT1, NOPARITY, 2048);
	com_open(COM2,BAUD(38400), DATABIT8,STOPBIT1, NOPARITY, 2048);

	set_com_getc_hook(COM1, Hook_Com);
	set_com_getc_hook(COM1, Hook_Com);

	protocol = new protocol_grp(COM1, 1, com_getc);
	protocol.grp[0].init_protocol(10, check_func, catch_func);
	protocol.grp[0].set_flag(1);



	int 	Rd;

	while(!kbhit())	{
		protocol.protocol_event();
		/*
		Rd = com_getc(COM1);
		if(Rd != BUFFER_EMPTY)	{
			com_putc(COM2, Rd);
		}
		*/
	}

	set_com_getc_hook(COM1, NULL);

	com_close();
	serial_close();

	delete protocol;

}

