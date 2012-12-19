#if !(defined(__FreeBSD__) || defined(__NetBSD__))
#include <fcntl.h>
#include "gamepad_libhid.h"

gamepad_libhid_t    *gamepad_libhid_open(const char *device,int io_flags)

{
    gamepad_libhid_t    *gp;
    int                 status;
    unsigned int        report_size;
    HIDInterfaceMatcher matcher = {0, 0, NULL, NULL, 0};
    
    gp = gamepad_libhid_init();

    printf("Device = %s\n",device);
    
    if ( (status=hid_init()) != HID_RET_SUCCESS )
    {
	fprintf(stderr,"gamepad_libhid_open(): hid_init() failed: %d\n",
		status);
	return NULL;
    }
    
    puts("HID initialized...");
    
    if ( (gp->hid_interface = hid_new_HIDInterface()) == NULL )
    {
	fputs("gamepad_libhid_open(): Could not allocate HIDInterface.\n",stderr);
	return NULL;
    }
    
    puts("Interface allocated...");
    
    /* Parse from device string */
    matcher.vendor_id = 0x046d;
    matcher.product_id = 0xc216;
    
    status = hid_force_open(gp->hid_interface,0,&matcher,3);
    if ( status != HID_RET_SUCCESS )
    {
	fprintf(stderr,"gamepad_libhid_open(): Could not open device: %d\n",
		status);
	
	hid_delete_HIDInterface(&gp->hid_interface);
	hid_cleanup();
	return NULL;
    }
    
    printf("Device opened.  Status = %d\n",status);
    fflush(stdout);
    
    hid_write_identification(stdout,gp->hid_interface);
    
    hid_dump_tree(stdout,gp->hid_interface);
    
    hid_get_report_size(gp->hid_interface,0,0,&report_size);
    printf("Report size = %u\n",report_size);
    return gp;
}


void    gamepad_libhid_close(gamepad_libhid_t *gp)

{
    hid_close(gp->hid_interface);
    hid_delete_HIDInterface(&gp->hid_interface);
    hid_cleanup();
}


gamepad_libhid_t    *gamepad_libhid_init(void)

{
    gamepad_libhid_t    *gp;
    
    gp = (gamepad_libhid_t *)malloc(sizeof(gamepad_libhid_t));
    return gp;
}


int     gamepad_libhid_read(gamepad_libhid_t *gp)

{
    int     bytes;

    /* How can the program extract device information like path components
       and buffer size? */
    return bytes;
}
#endif

