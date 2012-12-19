#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <sys/joystick.h>

#ifndef ABS
#define ABS(x)  ((x) >= 0 ? (x) : -(x))
#endif

typedef struct
{
    int     fd;
    int     io_flags;
    int     num_buttons;    /* Generally 2 */
    int     noise_max;      /* Maximum random change in x or y.  Used
				to filter out noise. */
    int     max_x;
    int     max_y;
    struct joystick joystick;
}   gamepad_analog_t;

/* gamepad_analog.c */
gamepad_analog_t *gamepad_analog_open(const char *device, int io_flags);
gamepad_analog_t *gamepad_analog_init(void);
int gamepad_analog_read(gamepad_analog_t *gp);
int gamepad_analog_x(gamepad_analog_t *gp);
int gamepad_analog_y(gamepad_analog_t *gp);
int gamepad_analog_z(gamepad_analog_t *gp);
int gamepad_analog_rx(gamepad_analog_t *gp);
int gamepad_analog_ry(gamepad_analog_t *gp);
int gamepad_analog_rz(gamepad_analog_t *gp);
int gamepad_analog_hat_switch(gamepad_analog_t *gp);
int gamepad_analog_max_x(gamepad_analog_t *gp);
int gamepad_analog_max_y(gamepad_analog_t *gp);
int gamepad_analog_max_z(gamepad_analog_t *gp);
int gamepad_analog_max_rx(gamepad_analog_t *gp);
int gamepad_analog_max_ry(gamepad_analog_t *gp);
int gamepad_analog_max_rz(gamepad_analog_t *gp);
int gamepad_analog_button(gamepad_analog_t *gp, int button);
int gamepad_analog_num_buttons(gamepad_analog_t *gp);
void gamepad_analog_close(gamepad_analog_t *gp);
void gamepad_analog_set_nonblock(gamepad_analog_t *gp);
void gamepad_analog_unset_nonblock(gamepad_analog_t *gp);
#endif

