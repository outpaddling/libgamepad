#if defined(__FreeBSD__) || defined(__NetBSD__)
#include "gamepad_analog.h"
#include "gamepad_usbhid.h"
#else
#include "gamepad_libhid.h"
#endif

typedef enum
{
    GAMEPAD_INTERFACE_AUTO,     /* Send to gamepad_open for auto-detection */
    GAMEPAD_INTERFACE_UNKNOWN,  /* Return code when can't be determined */
    GAMEPAD_INTERFACE_ANALOG,
    GAMEPAD_INTERFACE_USBHID,
    GAMEPAD_INTERFACE_LIBHID
}   gamepad_interface_t;

typedef struct
{
    /* Which interface */
    gamepad_interface_t interface;
    
#if defined(__FreeBSD__) || defined(__NetBSD__)
    gamepad_usbhid_t    *usbhid;
    gamepad_analog_t    *analog;
#else
    gamepad_libhid_t    *libhid;
#endif
}   gamepad_t;

/* gamepad.c */
gamepad_t *gamepad_open(const char *device, gamepad_interface_t interface, int io_flags);
int gamepad_read(gamepad_t *gp);
int gamepad_x(gamepad_t *gp);
int gamepad_y(gamepad_t *gp);
int gamepad_z(gamepad_t *gp);
int gamepad_rz(gamepad_t *gp);
int gamepad_hat_switch(gamepad_t *gp);
int gamepad_button(gamepad_t *gp, int button);
int gamepad_num_buttons(gamepad_t *gp);
void gamepad_close(gamepad_t *gp);
gamepad_interface_t gamepad_check_interface(const char *device);
int gamepad_max_x(gamepad_t *gp);
int gamepad_max_y(gamepad_t *gp);
int gamepad_max_z(gamepad_t *gp);
int gamepad_max_rz(gamepad_t *gp);
void gamepad_set_nonblock(gamepad_t *gp);
void gamepad_unset_nonblock(gamepad_t *gp);
void gamepad_internal_error(const char *format, ...);

