/* 
 * File:   car_def.h
 * Author: acer
 *
 * Created on 3 May, 2025, 10:39 AM
 */

#ifndef CAR_DEF_H
#define	CAR_DEF_H

#define EEPROM_ADDR 0x06
void display_dashboard(unsigned char event[],unsigned char speed);

void log_event(unsigned char event[],unsigned char speed);
void clear_display();
unsigned char login(unsigned char key,unsigned char reset_flag);
unsigned char menu_screen(unsigned char key,unsigned char reset_flag,unsigned char *control_flag);
void set_time();
unsigned char change_passwd();
unsigned char view_log();
unsigned char clear_log();
unsigned char download_log();
unsigned char change_time();
#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CAR_DEF_H */

