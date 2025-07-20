//black box project definition

//display dashboard definition
#include "main.h"
#include <stdbool.h>

unsigned char clock_reg[3];
char time[6];  // "HHMMSS"
char log[11]; //"HHMMSSEVP"
char log_pos=0;//to store the log pos
unsigned int log_count =0;
unsigned char sec;
unsigned char return_time;
unsigned char *menu[]={"View Log","Clear Log","Download Log","Change passwd","Set time"};
unsigned char menu_pos;
unsigned char clear_flag = 0;
unsigned int menu_delay=0;


static void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
    
   
}
static void display_time()
{
    //BCD Format 
    //clock_reg[0] = HH
    //clock_reg[1] = MM
    //clock_reg[2] = SS
    get_time();
    clcd_putch(time[0], LINE2(2)); 
    clcd_putch(time[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(time[2], LINE2(5)); 
    clcd_putch(time[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(time[4], LINE2(8)); 
    clcd_putch(time[5], LINE2(9));
    


    
}


void display_dashboard(unsigned char event[],unsigned char speed)
{
    //line 1 of clcd display the title message
    clcd_print("TIME     E  SP",LINE1(2));
    //display time
    display_time();
    //display event
    clcd_print(event,LINE2(11));
    
    
    //display the speed(0-99)
    clcd_putch((speed/10)+'0',LINE2(14));
    clcd_putch((speed%10)+'0',LINE2(15));

}

void store_event(){
    
   unsigned char addr;

    addr = 0x05 + log_pos * 10;
    ext_eeprom_24C02_str_write(addr, log);

    log_pos = (log_pos + 1) % 10;

    if (log_count < 10)
        log_count++;
    
    
    
    
}

unsigned char clear_log(){
    if(clear_flag==0){
        clear_flag=1;
        clcd_print("LOG CLEARED",LINE1(2));
        __delay_ms(1000);
        clear_display();
        return MAIN_MENU;
        log_pos=0;
        
    }
    else{
        clcd_print("NO LOG PRESENT",LINE1(0));
        __delay_ms(1000);
        clear_display();
        return MAIN_MENU;
    }
}


void log_event(unsigned char event[],unsigned char speed){
    get_time();
     clear_flag = 0;
    strncpy(log,time,6);
    
    strncpy(&log[6],event,2);
    
    log[8] = (speed/10)+'0';
    
    log[9] = (speed%10)+'0';
    
    log[10] = '\0';
    store_event();
    
}

unsigned char login(unsigned char key,unsigned char reset_flag){
    static unsigned char user_passwd[4];
    static unsigned char attempt_left;
    static unsigned char i;
    
    if(reset_flag==RESET_PASSWD){
        i=0;
        attempt_left=3;
        user_passwd[0]='\0';
        user_passwd[1]='\0';
        user_passwd[2]='\0';
        user_passwd[3]='\0';
        key = ALL_RELEASED; //to make sure the key press is not considered as passwd prompt
        return_time=5;
    }
    
    if(key==SW4 && i<4){
        clcd_putch('*',LINE2(4+i));
        user_passwd[i]='1';
        i++;
        return_time=5;
    }
    else if(key==SW5 && i<4){
        clcd_putch('*',LINE2(4+i));
        user_passwd[i]='0';
        i++;
        return_time=5;
    }
    if(return_time ==0){
        //change to dashboadscreen
        return RETURN_BACK;
    }
    if(i==4){
        char s_passwd[4];
        //read stored password from the user
        for(int j=0;j<4;j++){   //to read 4 bytes from eerprom
            s_passwd[j]=ext_eeprom_24C02_read(j);
        }
        //compare the stored password and user password
        if(strncmp(user_passwd,s_passwd,4)==0){
            clear_display();
             clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            clcd_print("LOGIN SUCCESS",LINE1(1));
            __delay_ms(3000);
            //change to main menu screen
            return LOGIN_SUCCESS;
            
        }
        else{
            attempt_left--;
            if(attempt_left==0){
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
                clcd_print("YOU ARE BLOCKED",LINE1(0));
                clcd_print("WAIT FOR",LINE2(0));
                clcd_print("Secs",LINE2(12));
                //wait for 60 seconds
                sec =60;
                while(sec){
                    clcd_putch((sec/10)+'0',LINE2(9));
                    clcd_putch((sec%10)+'0',LINE2(10));
                }
                
                attempt_left=3;
            }
            else{
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("WRONG PASSWORD",LINE1(0));
                clcd_putch(attempt_left+'0',LINE2(0));
                clcd_print("ATTEMPT LEFT ",LINE2(3));
                
                __delay_ms(3000);                
            }
            clear_display();
            clcd_print("ENTER PASSWORD",LINE1(1));
            clcd_write(LINE2(4),INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i=0;
        }
    }
}
unsigned char change_passwd()
{
    unsigned char key;
    static unsigned char new_passwd[4];
    static unsigned char in = 0;
    unsigned char addr = 0x00;
    static unsigned char new_passwd1[4];
    static unsigned char in1 = 0;

    clear_display();
    clcd_print("ENTER NEW PASS", LINE1(0));
    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);  // Show cursor once
    clcd_write(LINE2(4), INST_MODE);               // Set cursor to first digit

    while (1)
    {
        key = read_digital_keypad(STATE);
        __delay_ms(100);

        if (return_time == 0)
        {
            return RETURN_BACK;
        }

        return_time = 5;

        if (key == SW4 && in < 4)
        {
            clcd_putch('*', LINE2(4 + in));
            new_passwd[in++] = '1';
        }
        else if (key == SW5 && in < 4)
        {
            clcd_putch('*', LINE2(4 + in));
            new_passwd[in++] = '0';
        }

        if (in == 4)
        {
            clear_display();
            clcd_print("RE-ENTER PASS", LINE1(0));
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            clcd_write(LINE2(4), INST_MODE);
            in1 = 0;
            key = ALL_RELEASED;

            while (in1 < 4)
            {
                key = read_digital_keypad(STATE);
                __delay_ms(100);

                if (key == SW4)
                {
                    clcd_putch('*', LINE2(4 + in1));
                    new_passwd1[in1++] = '1';
                    return_time = 5;
                }
                else if (key == SW5)
                {
                    clcd_putch('*', LINE2(4 + in1));
                    new_passwd1[in1++] = '0';
                    return_time = 5;
                }
            }

            if (strncmp(new_passwd, new_passwd1, 4) == 0)
            {
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                clcd_print("PASSWORD CHANGED", LINE1(0));
                __delay_ms(2000);

                for (int j = 0; j < 4; j++)
                {
                    ext_eeprom_24C02_byte_write(addr + j, new_passwd[j]);
                }

                in = 0;
                in1 = 0;
                return CHANGE_SUCCESS;
            }
            else
            {
                clear_display();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                clcd_print("PASSWORD DO NOT", LINE1(0));
                clcd_print("MATCH", LINE2(5));
                __delay_ms(2000);

                in = 0;
                in1 = 0;
                return RETURN_BACK;
            }
        }
    }

    return RETURN_BACK;
}


unsigned char download_log()
{
    unsigned char i, j;
    unsigned char addr;
    char log_buf[11];
    char time_disp[9];
    char event[3];
    char speed[3];
    return_time = 5;
    if (clear_flag == 0) // Logs exist
    {
        __delay_ms(200);
        __delay_ms(300);

        puts("# TIME     E  SP\n");

        for (i = 0; i < log_count; i++)
        {
            addr = 0x05 + i * 10;
            for (j = 0; j < 10; j++)
            {
                log_buf[j] = ext_eeprom_24C02_read(addr + j);
            }
            log_buf[10] = '\0';

            // Format time
            time_disp[0] = log_buf[0];
            time_disp[1] = log_buf[1];
            time_disp[2] = ':';
            time_disp[3] = log_buf[2];
            time_disp[4] = log_buf[3];
            time_disp[5] = ':';
            time_disp[6] = log_buf[4];
            time_disp[7] = log_buf[5];
            time_disp[8] = '\0';

            // Event
            event[0] = log_buf[6];
            event[1] = log_buf[7];
            event[2] = '\0';

            // Speed
            speed[0] = log_buf[8];
            speed[1] = log_buf[9];
            speed[2] = '\0';

            // Print formatted log
            putchar(i + '0');        // Log number
            putchar(' ');
            puts(time_disp);
            putchar(' ');
            puts(event);
            putchar(' ');
            puts(speed);
            putchar('\r\n');
            
        }
    }
    else
    {
        puts("NO LOG FOUND\n");
    }
    
    return MAIN_MENU;
}


unsigned char menu_screen(unsigned char key, unsigned char reset_flag, unsigned char *control_flag) {
    static unsigned int menu_delay = 0;
    static unsigned char prev_key = ALL_RELEASED;
    static unsigned char longpress = 0;
    static unsigned char short_press_done = 0; // Added to track short press

    if (reset_flag == RESET_MENU) {
        menu_pos = 0;
        return_time = 5;
        menu_delay = 0;
        longpress = 0;
        prev_key = ALL_RELEASED;
        short_press_done = 0; // Reset short press flag
        return MAIN_MENU;
    }

    if (key == SW4 || key == SW5) {
        return_time = 5;
        if (key == prev_key) {
            menu_delay++;

            if (menu_delay > 20) {
                if (key == SW4) {
                    longpress = 1;
                    __delay_us(500);
                    switch (menu_pos) {
                        case 0: *control_flag = VIEW_LOG_SCREEN; break;
                        case 1: *control_flag = CLEAR_LOG; break;
                        case 2: *control_flag = DOWNLOAD_LOG; break;
                        case 3: *control_flag = CHANGE_PASSWD; break;
                        case 4: *control_flag = CHANGE_TIME; break;
                    }
                } else if (key == SW5) {
                    longpress = 1;
                    clear_display();
                    clcd_print("LOGGING OUT", LINE1(2));
                    __delay_ms(2000);
                    clear_display();
                    return RETURN_BACK;
                }
            }
        } else {
            // New key press
            menu_delay = 1; // Start counting
            prev_key = key;
            short_press_done = 0; // Reset short press flag
        }
    } else {
        // No key pressed
        if (prev_key != ALL_RELEASED && menu_delay <= 20 && longpress == 0) {
             // This is a short press, and long press didn't happen
            if (prev_key == SW4 && menu_pos > 0) {
                menu_pos--;
                clear_display();
            } else if (prev_key == SW5 && menu_pos < 4) {
                menu_pos++;
                clear_display();
            }
            short_press_done = 1; //prevent repeated short press
        }
        menu_delay = 0;
        prev_key = ALL_RELEASED;
        longpress = 0;
    }

    // Display menu
    if (menu_pos == 4) {
        clcd_putch('*', LINE2(0));
        clcd_print(menu[menu_pos - 1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
    } else {
        clcd_putch('*', LINE1(0));
        clcd_print(menu[menu_pos], LINE1(2));
        clcd_print(menu[menu_pos + 1], LINE2(2));
    }

    if (return_time == 0) {
        return RETURN_BACK;
    }

    return MAIN_MENU;
}

void set_time(void)
{
    unsigned char time_fields[3]; // HH, MM, SS
    unsigned char field = 2;
    unsigned char key, old_key = 0;
    unsigned int delay = 0;
    unsigned char blink = 0;

    // Read current time
    time_fields[0] = read_ds1307(HOUR_ADDR);
    time_fields[1] = read_ds1307(MIN_ADDR);
    time_fields[2] = read_ds1307(SEC_ADDR);
    clear_display();
    clcd_print("Set Time:", LINE1(0)); // Print static header only once

    while (1)
    {
        blink = !blink;

        // Display time values (reprint each time, but no flicker)
        for (unsigned char i = 0; i < 3; i++)
        {
            unsigned char pos = i * 3;
            if (i == field && blink)
            {
                clcd_putch(' ', LINE2(pos));
                clcd_putch(' ', LINE2(pos + 1));
            }
            else
            {
                clcd_putch(((time_fields[i] >> 4) & 0x0F) + '0', LINE2(pos));
                clcd_putch((time_fields[i] & 0x0F) + '0', LINE2(pos + 1));
            }

            if (i < 2)
                clcd_putch(':', LINE2(pos + 2));
        }

        // Read key
        key = read_digital_keypad(LEVEL);

        // UP key
        if (key == SW4)
        {
            delay++;
            if (delay > 10) // ~1s long press
            {
                write_ds1307(HOUR_ADDR, time_fields[0]);
                write_ds1307(MIN_ADDR, time_fields[1]);
                write_ds1307(SEC_ADDR, time_fields[2]);

                clear_display();
                clcd_print("Time Set Success", LINE1(0));
                __delay_ms(2000);
                return;
            }
        }
        else if (key == ALL_RELEASED && old_key == SW4)
        {
            if (delay > 0 && delay <= 10) // Short press
            {
                unsigned char value = time_fields[field];
                unsigned char tens = (value >> 4) & 0x0F;
                unsigned char ones = value & 0x0F;

                ones++;
                if (ones > 9)
                {
                    ones = 0;
                    tens++;
                }

                if (field == 0)
                {
                    if (tens > 2 || (tens == 2 && ones > 3))
                    {
                        tens = 0;
                        ones = 0;
                    }
                }
                else
                {
                    if (tens > 5)
                    {
                        tens = 0;
                        ones = 0;
                    }
                }

                time_fields[field] = (tens << 4) | ones;
            }

            delay = 0;
        }

        // DOWN key
        if (key == SW5 && old_key != SW5)
        {
            field = (field + 1) % 3;
        }

        old_key = key;
        __delay_ms(200);
    }
}



unsigned char view_log()
{
    unsigned char i = 0, j;
    unsigned char addr;
    char log_buf[11];
    char time_disp[9];
    char event[3];
    char speed[3];
    unsigned int sw4_count = 0;
    unsigned int sw5_count = 0;
    unsigned char key;

    if (clear_flag == 0) // logs exist
    {
        __delay_ms(200);
        __delay_ms(300);
        while (1)
        {
            // Read log entry at position i
            addr = 0x05 + i * 10;
            for (j = 0; j < 10; j++)
            {
                log_buf[j] = ext_eeprom_24C02_read(addr + j);
            }
            log_buf[10] = '\0';

            // Parse time
            time_disp[0] = log_buf[0];
            time_disp[1] = log_buf[1];
            time_disp[2] = ':';
            time_disp[3] = log_buf[2];
            time_disp[4] = log_buf[3];
            time_disp[5] = ':';
            time_disp[6] = log_buf[4];
            time_disp[7] = log_buf[5];
            time_disp[8] = '\0';

            // Event and Speed
            event[0] = log_buf[6];
            event[1] = log_buf[7];
            event[2] = '\0';

            speed[0] = log_buf[8];
            speed[1] = log_buf[9];
            speed[2] = '\0';

            // Display log
            clear_display();
            clcd_print("# TIME     E  SP", LINE1(0));
            clcd_putch(i + '0', LINE2(0));  // Log number (1?10)
            clcd_print(time_disp, LINE2(2));
            clcd_print(event, LINE2(11));
            clcd_print(speed, LINE2(14));

            sw4_count = 0;
            sw5_count = 0;

            while (1)
            {
                key = read_digital_keypad(LEVEL);

                if (key == SW4)
                {
                    return_time = 5;
                    sw4_count++;
                    __delay_ms(100);
                    if (sw4_count > 20) // long press
                    {
                        clear_display();
                        while (read_digital_keypad(LEVEL) != ALL_RELEASED); // <- Wait for release
                        return MAIN_MENU;
                    }
                }
                else if (sw4_count > 0 && sw4_count <= 20)
                {
                    return_time = 5;
                    // Short press
                    if(i>=log_count-1){
                        i=0;
                    }
                    else
                    {
                    i = i+1;
                    }
                    break;
                }

                if (key == SW5)
                {
                    
                    sw5_count++;
                    __delay_ms(100);
                    if (sw5_count > 20) // long press
                    {
                        return_time = 5;
                       
                        clear_display();
                        
                    clcd_print("LOGGING OUT", LINE1(2));
                    __delay_ms(2000);
                    clear_display();
                        return DASHBOARD_SCREEN;
                    }
                }
                else if (sw5_count > 0 && sw5_count <= 20)
                {
                    return_time = 5;
                    // Short press
                    if (i == 0)
                        i = 0;
                    else
                        i--;
                    break;
                }
            }
        }
    }
    else
    {
        clear_display();
        clcd_print("NO LOG FOUND", LINE1(1));
        __delay_ms(3000);
        clear_display();
        return MAIN_MENU;
    }

    return MAIN_MENU;
}

void clear_display(){
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            __delay_us(500);
}