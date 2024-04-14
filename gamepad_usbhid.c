#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <dev/usb/usb.h>

#if defined(__FreeBSD__)
#include <dev/usb/usb_ioctl.h>
#endif

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sysexits.h>
#include "gamepad_usbhid.h"

#define USB_GAMEPAD "/dev/uhid0"

/***************************************************************************
 *  Description:
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  History: 
 *  Date        Name        Modification
 *
 ***************************************************************************/

/**
 *  \brief  
 *  \param  
 *  \param
 *  \param
 *  \author Jason W. Bacon
 */

gamepad_usbhid_t   *gamepad_usbhid_open(const char *file,int io_flags)

{
    gamepad_usbhid_t   *gp;
    int         usage,
		usage_page;
    
    /* Allocate and initialize gp structure */
    gp = gamepad_usbhid_init();
    
    /* Open device node */
    gp->fd = open(USB_GAMEPAD,io_flags);
    if ( gp->fd < 0 )
    {
	EPRINTF("open() failed: %s\n", strerror(errno));
	free(gp);
	return NULL;
    }
    
    /* Initialize usbhid library, specifying usage table pathname. 
       NULL means use system default table. */
    hid_init(gp->table);
    
    /* Get report descriptor for HID device.  This tells us the
       format of the data reported by the device. */
    gp->hid_report_descriptor = hid_get_report_desc(gp->fd);
    if ( gp->hid_report_descriptor == NULL )
    {
	EPRINTF("hid_get_report_desc() failed: %s.\n", strerror(errno));
	return NULL;
    }

    /* Parse report descriptor to determine usages (features) of the device */
    gp->hid_data = hid_start_parse(gp->hid_report_descriptor,1<<hid_input,0);
    if ( gp->hid_data == NULL )
    {
	EPRINTF("hid_start_parse() failed: %s.\n", strerror(errno));
	return NULL;
    }
    
    gp->hid_items = (hid_item_t *)malloc(gp->item_array_size * sizeof(hid_item_t));

    for (gp->item_count=0; hid_get_item(gp->hid_data,gp->hid_items+gp->item_count) > 0; )
    {
	usage_page = gp->hid_items[gp->item_count]._usage_page >> 16;
	usage = gp->hid_items[gp->item_count].usage;
    
	/* Record indexes of known usages so they can be accessed
	   directly. */
	gamepad_usbhid_check_usage_code(gp);
	
	++gp->item_count;
	
	/* Expand item array if necessary. */
	if ( gp->item_count == gp->item_array_size )
	{
	    gp->item_array_size <<= 1;  /* Double array size */
	    gp->hid_items = (hid_item_t *)realloc(gp->hid_items,
					gp->item_array_size * sizeof(hid_item_t));
	}
    }
    
    /* Free mem allocated by hid_start_parse() */
    hid_end_parse(gp->hid_data);
    
    /* Set for read() to return immediately, otherwise it blocks
       until there's an input event. */
    if ( io_flags & O_NONBLOCK )
    {
	gamepad_usbhid_set_nonblock(gp);
    }
    
    /* Allocate I/O buffer for reading/writing reports. */
    gp->report_size = hid_report_size(gp->hid_report_descriptor,hid_input,0);
    gp->buff = malloc(gp->report_size);
    
    return gp;
}


gamepad_usbhid_t *gamepad_usbhid_init(void)

{
    gamepad_usbhid_t    *gp;
    
    gp = (gamepad_usbhid_t *)malloc(sizeof(gamepad_usbhid_t));
    
    gp->item_array_size = 64;   /* Initial size.  gamepad_usbhid_open()
				    uses realloc() if necessary. */
    gp->num_buttons = 0;
    gp->table = NULL;           /* Use system default usage table */
    gp->fd = -1;
    return gp;
}


void    gamepad_usbhid_check_usage_code(gamepad_usbhid_t *gp)

{
    int     c,
	    button_usage_code;

    /* Record indexes of known usages so they can be accessed directly. */
    switch(gp->hid_items[gp->item_count].usage)
    {
	case    HID_USAGE_X:
	    gp->item_index_x = gp->item_count;
	    break;
	case    HID_USAGE_Y:
	    gp->item_index_y = gp->item_count;
	    break;
	case    HID_USAGE_Z:
	    gp->item_index_z = gp->item_count;
	    break;
	case    HID_USAGE_RZ:
	    gp->item_index_rz = gp->item_count;
	    break;
	case    HID_USAGE_HAT_SWITCH:
	    gp->item_index_hat_switch = gp->item_count;
	    break;
	default:
	    break;
    }
    
    for (c=1; c <= GAMEPAD_USBHID_MAX_BUTTONS; ++c)
    {
	button_usage_code = 0x90000 + c;
	if ( gp->hid_items[gp->item_count].usage == button_usage_code )
	{
	    gp->item_index_button[c] = gp->item_count;
	    ++gp->num_buttons;
	}
    }
}


int     gamepad_usbhid_num_buttons(gamepad_usbhid_t *gp)

{
    return gp->num_buttons;
}


void    gamepad_usbhid_close(gamepad_usbhid_t *gp)

{
    /* Free mem allocated by hid_get_report_desc() */
    hid_dispose_report_desc(gp->hid_report_descriptor);
    
    // usbhid
    close(gp->fd);
    gp->fd = -1;
    free(gp);
}


int     gamepad_usbhid_read(gamepad_usbhid_t *gp)

{
    int     bytes;
    
    //if ( (bytes = read(gp->fd,gp->buff,gp->report_size)) != gp->report_size )
    if ( (bytes = read(gp->fd,gp->buff,gp->report_size)) == -1 )
    {
	if ( errno == EAGAIN )
	    return 0;
	else
	    return -1;
    }
    return bytes;
}


int     gamepad_usbhid_x(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_x);
}


int     gamepad_usbhid_y(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_y);
}


int     gamepad_usbhid_z(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_z);
}


int     gamepad_usbhid_rx(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_rx);
}


int     gamepad_usbhid_ry(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_ry);
}


int     gamepad_usbhid_rz(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_rz);
}


int     gamepad_usbhid_get_max(hid_item_t hid_items[],int index)

{
    if ( index >= 0 )
	return hid_items[index].logical_maximum;
    else
	return 0;
}


int     gamepad_usbhid_max_x(gamepad_usbhid_t *gp)

{
    return gamepad_usbhid_get_max(gp->hid_items,gp->item_index_x);
}


int     gamepad_usbhid_max_y(gamepad_usbhid_t *gp)

{
    return gamepad_usbhid_get_max(gp->hid_items,gp->item_index_y);
}


int     gamepad_usbhid_max_z(gamepad_usbhid_t *gp)

{
    return gamepad_usbhid_get_max(gp->hid_items,gp->item_index_z);
}


int     gamepad_usbhid_max_rx(gamepad_usbhid_t *gp)

{
    return gamepad_usbhid_get_max(gp->hid_items,gp->item_index_rx);
}


int     gamepad_usbhid_max_ry(gamepad_usbhid_t *gp)

{
    return gamepad_usbhid_get_max(gp->hid_items,gp->item_index_ry);
}


int     gamepad_usbhid_max_rz(gamepad_usbhid_t *gp)

{
    return gamepad_usbhid_get_max(gp->hid_items,gp->item_index_rz);
}


int     gamepad_usbhid_hat_switch(gamepad_usbhid_t *gp)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_hat_switch);
}


int     gamepad_usbhid_button(gamepad_usbhid_t *gp,int button)

{
    return hid_get_data(gp->buff,gp->hid_items+gp->item_index_button[button]);
}


int     gamepad_usbhid_get_val_from_usage_name(gamepad_usbhid_t *gp,const char *name)

{
    int     c;
    
    for (c=0; c<gp->item_count; ++c)
    {
	if ( strcmp(hid_usage_in_page(gp->hid_items[c].usage),name) == 0 )
	    return hid_get_data(gp->buff,gp->hid_items+c);
    }
    return 0;
}


int     gamepad_usbhid_get_val_from_usage_code(gamepad_usbhid_t *gp,int code)

{
    int     c;
    
    for (c=0; c<gp->item_count; ++c)
    {
	if ( gp->hid_items[c].usage == code )
	    return hid_get_data(gp->buff,gp->hid_items+c);
    }
    return 0;
}


void    gamepad_usbhid_set_nonblock(gamepad_usbhid_t *gp)

{
#if defined(__FreeBSD__)
/*
    const int   one = 1;

    if ( gp->fd != -1 )
    {
	if (ioctl(gp->fd, USB_SET_IMMED, &one) < 0)
	{
	    if (errno == EOPNOTSUPP)
		fprintf(stderr,"Device does not support immediate mode.\n");
	    else
		fprintf(stderr,"Cannot set immediate mode: %d\n",errno);
	}
    }
*/
    printf("Setting immediate mode...\n");
    hid_set_immed(gp->fd, 1);
#else
#warning gamepad_usbhid_set_nonblock() is undefined for this platform.
#endif
}


void    gamepad_usbhid_unset_nonblock(gamepad_usbhid_t *gp)

{
}
#endif

