#if defined(__FreeBSD__) || defined(__NetBSD__)
#include <usbhid.h>

#define EPRINTF(...) \
    fprintf(stderr, "Error: %s(): ", __func__); \
    fprintf(stderr, __VA_ARGS__)

#define GAMEPAD_USBHID_MAX_BUTTONS  32

enum
{
    HID_USAGE_X = 0x10030,
    HID_USAGE_Y = 0x10031,
    HID_USAGE_Z = 0x10032,
    HID_USAGE_RX = 0x10033,
    HID_USAGE_RY = 0x10034,
    HID_USAGE_RZ = 0x10035,
    HID_USAGE_HAT_SWITCH = 0x10039,
    HID_USAGE_BUTTON1 = 0x90001,
    HID_USAGE_BUTTON2 = 0x90002,
    HID_USAGE_BUTTON3 = 0x90003,
    HID_USAGE_BUTTON4 = 0x90004,
    HID_USAGE_BUTTON5 = 0x90005,
    HID_USAGE_BUTTON6 = 0x90006,
    HID_USAGE_BUTTON7 = 0x90007,
    HID_USAGE_BUTTON8 = 0x90008,
    HID_USAGE_BUTTON9 = 0x90009,
    HID_USAGE_BUTTON10 = 0x90010,
    HID_USAGE_BUTTON11 = 0x90011,
    HID_USAGE_BUTTON12 = 0x90012
};

typedef struct
{
    int         num_buttons;
    int         fd;
    int         item_count;
    int         item_array_size;
    int         report_size;
    char        *buff;      /* read/write buffer */
    char        *table;     /* HID name table */
    report_desc_t   hid_report_descriptor;
    hid_data_t      hid_data;
    hid_item_t  *hid_items;
    int         item_index_x;
    int         item_index_y;
    int         item_index_z;
    int         item_index_rx;
    int         item_index_ry;
    int         item_index_rz;
    int         item_index_hat_switch;
    int         item_index_button[GAMEPAD_USBHID_MAX_BUTTONS+1];
}   gamepad_usbhid_t;

/* gamepad_usbhid.c */
gamepad_usbhid_t *gamepad_usbhid_open(const char *file, int io_flags);
gamepad_usbhid_t *gamepad_usbhid_init(void);
void gamepad_usbhid_check_usage_code(gamepad_usbhid_t *gp);
int gamepad_usbhid_num_buttons(gamepad_usbhid_t *gp);
void gamepad_usbhid_close(gamepad_usbhid_t *gp);
int gamepad_usbhid_read(gamepad_usbhid_t *gp);
int gamepad_usbhid_x(gamepad_usbhid_t *gp);
int gamepad_usbhid_y(gamepad_usbhid_t *gp);
int gamepad_usbhid_z(gamepad_usbhid_t *gp);
int gamepad_usbhid_rx(gamepad_usbhid_t *gp);
int gamepad_usbhid_ry(gamepad_usbhid_t *gp);
int gamepad_usbhid_rz(gamepad_usbhid_t *gp);
int gamepad_usbhid_get_max(hid_item_t hid_items[], int index);
int gamepad_usbhid_max_x(gamepad_usbhid_t *gp);
int gamepad_usbhid_max_y(gamepad_usbhid_t *gp);
int gamepad_usbhid_max_z(gamepad_usbhid_t *gp);
int gamepad_usbhid_max_rx(gamepad_usbhid_t *gp);
int gamepad_usbhid_max_ry(gamepad_usbhid_t *gp);
int gamepad_usbhid_max_rz(gamepad_usbhid_t *gp);
int gamepad_usbhid_hat_switch(gamepad_usbhid_t *gp);
int gamepad_usbhid_button(gamepad_usbhid_t *gp, int button);
int gamepad_usbhid_get_val_from_usage_name(gamepad_usbhid_t *gp, const char *name);
int gamepad_usbhid_get_val_from_usage_code(gamepad_usbhid_t *gp, int code);
void gamepad_usbhid_set_nonblock(gamepad_usbhid_t *gp);
void gamepad_usbhid_unset_nonblock(gamepad_usbhid_t *gp);
#endif

