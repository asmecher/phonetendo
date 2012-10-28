#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <linux/input.h>

#define PKEY_RELEASE 0
#define PKEY_PRESS 1

#define PKEY_MAIL 107
#define PKEY_HOLD 158
#define PKEY_MUTE 77
#define PKEY_PHONEBOOK 71
#define PKEY_CONFERENCE 72
#define PKEY_PRIVACY 73
#define PKEY_3 2
#define PKEY_2 3
#define PKEY_1 4
#define PKEY_6 5
#define PKEY_5 6
#define PKEY_4 7
#define PKEY_9 8
#define PKEY_8 9
#define PKEY_7 10
#define PKEY_STAR 167
#define PKEY_0 11
#define PKEY_POUND 55
#define PKEY_MENU 60
#define PKEY_STATUS 106
#define PKEY_BACK 103
#define PKEY_CAPS 28
#define PKEY_MY_CONTENT 59
#define PKEY_MY_APPLICATIONS 105
#define PKEY_REDIAL 79
#define PKEY_SPEAKER 80
#define PKEY_CALL 108
#define PKEY_UP 62
#define PKEY_DOWN 102
#define PKEY_LEFT 76
#define PKEY_RIGHT 75
#define PKEY_OK 81

extern bool controller_open();
extern void controller_close();
extern struct input_event *controller_read();

#endif
