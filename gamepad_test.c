#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <fcntl.h>
#include "gamepad.h"

#define USBHID_GAMEPAD "/dev/uhid0"
#define ANALOG_GAMEPAD "/dev/joy0"
#define LIBHID_GAMEPAD "046d:c216"

void    usage(char *prog_name);

int     main(int argc,char *argv[])

{
    int         c;
    gamepad_t   *gp;
    char        *device;

    if ( argc != 2 )
	usage(argv[0]);

    device = argv[1];
    
    if ( (gp = gamepad_open(device,GAMEPAD_INTERFACE_AUTO,
		O_RDONLY)) == NULL )
    {
	fprintf(stderr,"Could not open %s\n",device);
	return EX_UNAVAILABLE;
    }
    
    printf("Buttons = %d\n",gamepad_num_buttons(gp));
    do
    {
	puts("Press button 1 to quit...");
	gamepad_read(gp);
	printf("X: %3d  Y: %3d  Z: %3d  Rz: %3d  Hat: %3d\n",
	    gamepad_x(gp),gamepad_y(gp),gamepad_z(gp),gamepad_rz(gp),
	    gamepad_hat_switch(gp));
	for (c=1; c<=12; ++c)
	    printf("%d ",gamepad_button(gp,c));
	putchar('\n');
    }   while ( gamepad_button(gp,1) == 0 );

    gamepad_close(gp);
    
    return EX_OK;
}


void    usage(char *prog_name)

{
    fprintf(stderr,"Usage: %s <device>\n\nExamples:\n\n",prog_name);
    fprintf(stderr,"\t%s %s (BSD uhid driver and libusbhid)\n",prog_name,USBHID_GAMEPAD);
    fprintf(stderr,"\t%s %s (BSD joy driver)\n",prog_name,ANALOG_GAMEPAD);
    fprintf(stderr,"\t%s %s (libhid) \n",prog_name,LIBHID_GAMEPAD);
    exit(EX_USAGE);
}

