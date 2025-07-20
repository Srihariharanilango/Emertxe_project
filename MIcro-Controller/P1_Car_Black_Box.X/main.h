#include <xc.h>

#include<string.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include"car_def.h"
#include "timers.h"
#include "EEPROM.h"
#include "uart.h"


#define DASHBOARD_SCREEN 0x00
#define LOGIN_SCREEN 0x01
#define MAIN_MENU 0x02

#define RESET_PASSWD 0x11
#define RESET_NOTHING 0x22
#define CHANGE_SUCCESS 0x23

#define RESET_TIME        0x88
#define SET_TIME_FLAG        0x07
#define TASK_SUCCESS 0x43
#define TASK_FAIL 0x34

#define LONG_PRESS_DELAY 1000
#define RETURN_BACK 0x33
#define LOGIN_SUCCESS 0x44
#define RESET_MENU  0x55
#define VIEW_LOG_SCREEN 0x66
#define CLEAR_LOG 0x12
#define DOWNLOAD_LOG 0x13
#define CHANGE_PASSWD 0x14
#define CHANGE_TIME 0x15