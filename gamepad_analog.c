#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "gamepad_analog.h"

gamepad_analog_t    *gamepad_analog_open(const char *device,int io_flags)

{
    gamepad_analog_t    *gp;

    gp = gamepad_analog_init();
    gp->io_flags = io_flags;
    
    if ( (gp->fd = open(device,io_flags)) < 0 )
    {
	free(gp);
	return NULL;
    }
    
    return gp;
}


gamepad_analog_t    *gamepad_analog_init(void)

{
    gamepad_analog_t    *gp;
    
    gp = (gamepad_analog_t *)malloc(sizeof(gamepad_analog_t));
    gp->num_buttons = 2;
    gp->noise_max = 4;
    gp->max_x = 1000;
    gp->max_y = 1000;
    return gp;
}


int     gamepad_analog_read(gamepad_analog_t *gp)

{
    int     bytes;
    static struct joystick old_joystick = {0,0,0,0};
    
    bytes = read(gp->fd,&(gp->joystick),sizeof(gp->joystick));
    if ( bytes != sizeof(gp->joystick) )
	return -1;

    /* Simulate blocking I/O */
    if ( ! (gp->io_flags & O_NONBLOCK) ) 
    {
	while ( (old_joystick.b1 == gp->joystick.b1) &&
		(old_joystick.b2 == gp->joystick.b2) &&
		(ABS(old_joystick.x - gp->joystick.x) < gp->noise_max) &&
		(ABS(old_joystick.y - gp->joystick.y) < gp->noise_max) )
	{
	    bytes = read(gp->fd,&(gp->joystick),sizeof(gp->joystick));
	    usleep(50000);
	    if ( bytes != sizeof(gp->joystick) )
		return -1;
	}
    }
    
    old_joystick = gp->joystick;
    return bytes;
}


int     gamepad_analog_x(gamepad_analog_t *gp)

{
    return gp->joystick.x;
}


int     gamepad_analog_y(gamepad_analog_t *gp)

{
    return gp->joystick.y;
}


int     gamepad_analog_z(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_rx(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_ry(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_rz(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_hat_switch(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_max_x(gamepad_analog_t *gp)

{
    return gp->max_x;
}


int     gamepad_analog_max_y(gamepad_analog_t *gp)

{
    return gp->max_y;
}


int     gamepad_analog_max_z(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_max_rx(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_max_ry(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_max_rz(gamepad_analog_t *gp)

{
    return 0;
}


int     gamepad_analog_button(gamepad_analog_t *gp,int button)

{
    switch(button)
    {
	case    1:
	    return gp->joystick.b1;
	case    2:
	    return gp->joystick.b2;
	default:
	    return 0;
    }
}


int     gamepad_analog_num_buttons(gamepad_analog_t *gp)

{
    return 2;
}


void    gamepad_analog_close(gamepad_analog_t *gp)

{
    close(gp->fd);
    free(gp);
}


void    gamepad_analog_set_nonblock(gamepad_analog_t *gp)

{
    gp->io_flags |= O_NONBLOCK;
}


void    gamepad_analog_unset_nonblock(gamepad_analog_t *gp)

{
    gp->io_flags &= ~O_NONBLOCK;
}
#endif

