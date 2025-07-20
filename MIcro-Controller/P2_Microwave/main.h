#ifndef MAIN_H
#define	MAIN_H

#include "clcd.h"
#include "matrix_keypad.h"
#include "timers.h"
#include "microwave_def.h"
#include <string.h>
#include <xc.h>

#define POWER_ON_SCREEN                    0x10
#define MENU_SCREEN                        0x11
#define MICRO_MODE_SCREEN                  0x12
#define GRILL_MODE_SCREEN                  0x13
#define CONVENTION_MODE_SCREEN             0X14
#define START_MODE_SCREEN                  0x15
#define RESET_DISPLAY                      0x20
#define RESET_NOTHING                      0x21
#define SUCCESS                            0x01

#define FAN RC2;
#define BUZZER RC1;
#endif	/* MAIN_H */