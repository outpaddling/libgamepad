#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <stdarg.h>
#include "gamepad.h"

gamepad_t   *gamepad_open(const char *device,
			gamepad_interface_t interface,int io_flags)

{
    gamepad_t   *gp;
    
    /* Try to determine gamepad interface from device name */
    if ( interface == GAMEPAD_INTERFACE_AUTO )
    {
	interface = gamepad_check_interface(device);
	if ( interface == GAMEPAD_INTERFACE_UNKNOWN )
	{
	    fprintf(stderr,"Cannot determined interface type for %s.\n",
		    device);
	    return NULL;
	}
    }

    gp = (gamepad_t *)malloc(sizeof(gamepad_t));
    gp->interface = interface;
    
    switch(interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    gp->usbhid = gamepad_usbhid_open(device,io_flags);
	    if ( gp->usbhid == NULL )
	    {
		free(gp);
		return NULL;
	    }
	    break;
	case    GAMEPAD_INTERFACE_ANALOG:
	    gp->analog = gamepad_analog_open(device,io_flags);
	    if ( gp->analog == NULL )
	    {
		free(gp);
		return NULL;
	    }
	    break;
#else
	case    GAMEPAD_INTERFACE_LIBHID:
	    gp->libhid = gamepad_libhid_open(device,io_flags);
	    if ( gp->libhid == NULL )
	    {
		free(gp);
		return NULL;
	    }
	    break;
#endif
	default:
	    gamepad_internal_error("gamepad_open():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return gp;
}


int     gamepad_read(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_read(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_read(gp->analog);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_read():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_x(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_x(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_x(gp->analog);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_x():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_y(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_y(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_y(gp->analog);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_y():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_z(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_z(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_z(gp->analog);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_z():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_rz(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_rz(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_rz(gp->analog);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_rz():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_hat_switch(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_hat_switch(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_hat_switch(gp->analog);
#else
	case    GAMEPAD_INTERFACE_LIBHID:
#endif
	    break;
	default:
	    gamepad_internal_error("gamepad_hat_switch():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_button(gamepad_t *gp,int button)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_button(gp->usbhid,button);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_button(gp->analog,button);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_button():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_num_buttons(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return gamepad_usbhid_num_buttons(gp->usbhid);
	case    GAMEPAD_INTERFACE_ANALOG:
	    return gamepad_analog_num_buttons(gp->analog);
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_num_buttons():"
		"Internal error: Invalid interface: %d\n",gp->interface);
    }
    return 0;
}


void    gamepad_close(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    gamepad_usbhid_close(gp->usbhid);
	    break;
	case    GAMEPAD_INTERFACE_ANALOG:
	    gamepad_analog_close(gp->analog);
	    break;
#else
	case    GAMEPAD_INTERFACE_LIBHID:
	    gamepad_libhid_close(gp->libhid);
	    break;
#endif
	default:
	    gamepad_internal_error("gamepad_close():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
}


gamepad_interface_t gamepad_check_interface(const char *device)

{
    int     len = strlen(device);
    
    /*
     * If it looks like a USB vendor:product ID, use libhid. 
     * Valid device IDs are hexidecimal vendor and/or product IDs
     * in one of the following formats:
     *  :XXXX       Product ID only
     *  XXXX:       Vendor ID only
     *  XXXX:XXXX   Vendor and product ID
     */
    if ( ((len == 5) || (len == 9)) && 
	 ((device[0] == ':') || (device[4] == ':')) )
	return GAMEPAD_INTERFACE_LIBHID;

#if defined(__FreeBSD__) || defined(__NetBSD__)
    if ( memcmp(device,"/dev",4) == 0 )
    {
	if ( memcmp(device+5,"uhid",4) == 0 )
	    return GAMEPAD_INTERFACE_USBHID;
	if ( memcmp(device+5,"joy",3) == 0 )
	    return GAMEPAD_INTERFACE_ANALOG;
    }
#elif defined(__APPLE__)
#else
#warning "gamepad_check_interface() is not completely defined for this platform."
#endif
    return GAMEPAD_INTERFACE_UNKNOWN;
}


int     gamepad_max_x(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return 255;
	case    GAMEPAD_INTERFACE_ANALOG:
	    return 1000;
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_max_x():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_max_y(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return 255;
	case    GAMEPAD_INTERFACE_ANALOG:
	    return 1000;
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_max_y():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    fprintf(stderr,
	    "gamepad_open(): Internal error: Invalid interface: %d\n",
		gp->interface);
	    exit(EX_SOFTWARE);
	    break;
    }
    return 0;
}


int     gamepad_max_z(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return 255;
	case    GAMEPAD_INTERFACE_ANALOG:
	    return 1000;
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_max_z():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


int     gamepad_max_rz(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    return 255;
	case    GAMEPAD_INTERFACE_ANALOG:
	    return 1000;
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_max_rz():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
    return 0;
}


void    gamepad_set_nonblock(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    gamepad_usbhid_set_nonblock(gp->usbhid);
	    break;
	case    GAMEPAD_INTERFACE_ANALOG:
	    gamepad_analog_set_nonblock(gp->analog);
	    break;
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_set_nonblock():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
}


void    gamepad_unset_nonblock(gamepad_t *gp)

{
    switch(gp->interface)
    {
#if defined(__FreeBSD__) || defined(__NetBSD__)
	case    GAMEPAD_INTERFACE_USBHID:
	    gamepad_usbhid_unset_nonblock(gp->usbhid);
	    break;
	case    GAMEPAD_INTERFACE_ANALOG:
	    gamepad_analog_unset_nonblock(gp->analog);
	    break;
#endif
	case    GAMEPAD_INTERFACE_LIBHID:
	    break;
	default:
	    gamepad_internal_error("gamepad_unset_nonblock():"
		"Internal error: Invalid interface: %d\n",gp->interface);
	    break;
    }
}


void    gamepad_internal_error(const char *format,...)

{
    va_list ap;
    
    va_start(ap,format);
    vfprintf(stderr,format,ap);
    exit(EX_SOFTWARE);
}

