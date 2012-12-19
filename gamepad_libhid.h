#if !(defined(__FreeBSD__) || defined(__NetBSD__))
#include <hid.h>

typedef struct
{
    int             num_buttons;
    HIDInterface    *hid_interface;
}   gamepad_libhid_t;

/* gamepad_libhid.c */
gamepad_libhid_t *gamepad_libhid_open(const char *device, int io_flags);
void gamepad_libhid_close(gamepad_libhid_t *gp);
gamepad_libhid_t    *gamepad_libhid_init(void);
#endif

