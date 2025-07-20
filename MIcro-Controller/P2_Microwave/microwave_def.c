#include "main.h"

unsigned char operation_mode_flag = 0;
int time_arr[4] = {0};
int temp_arr[3] = {0};
unsigned char time_count = 0;
unsigned char temp_time_count = 0;
unsigned char menu_index = 0;  // 0 to 3 for 4 menu options

extern unsigned char control_flag;

void menu_screen(unsigned char reset_flag)
{
    if (reset_flag == RESET_DISPLAY)
    {
        clear_display();
        clcd_print("MODES", LINE1(5));
     __delay_ms(2000);
        
    }

    control_flag = MENU_SCREEN;
    
    
    //clear_display();
    clcd_print("1.Micro 2.Grill", LINE1(0));     
clcd_print("3.Conv 4.Reheat", LINE2(0));    

}

void micro_mode(unsigned char reset_flag)
{
    if (reset_flag == RESET_DISPLAY)
    {
        clear_display();
        clcd_print("Power = 900W", LINE2(3));
        __delay_ms(500);
    }

    if (operation_mode_flag == 0)
    {
        set_time_screen();
    }
}

void set_time_screen()
{
    unsigned char key, field = 's';
    static char blink_timer = 0, blink = 0;
    int key_count = 3;

    memset(time_arr, 0, sizeof(time_arr));
    clear_display();
    clcd_print("SET TIME <MM:SS>", LINE1(0));
    clcd_print("TIME : ", LINE2(0));

    clcd_putch(':', LINE2(9));
    clcd_print("*:CLEAR   #ENTER", LINE4(0));

    while (!operation_mode_flag)
    {
        key = read_matrix_keypad(STATE);

        if (key == '#')
        {
            operation_mode_flag = 1;
        }
        else if (key == '*')
        {
            memset(time_arr, 0, sizeof(time_arr));
            key_count = 3;
        }
        else if (key != ALL_RELEASED && key_count >= 0 && key <= 9)
        {
            if (key_count > 1)
            {
                time_arr[2] = time_arr[3];
                time_arr[3] = key;
                key_count--;
                field = 's';

                if ((time_arr[2] * 10 + time_arr[3]) >= 60)
                {
                    time_arr[2] = time_arr[3] = 0;
                    key_count = 3;
                    clcd_print("SECS Should < 60", LINE3(0));
                    __delay_ms(1000);
                    clcd_print("                ", LINE3(0));
                }
            }
            else
            {
                time_arr[0] = time_arr[1];
                time_arr[1] = key;
                key_count--;
                field = 'm';
            }
        }

        if (++blink_timer >= 5)
        {
            blink_timer = 0;
            blink = !blink;
        }

        if (field == 'm' && blink)
        {
            clcd_print("  ", LINE2(7));
        }
        else
        {
            clcd_putch('0' + time_arr[0], LINE2(7));
            clcd_putch('0' + time_arr[1], LINE2(8));
        }

        clcd_putch(':', LINE2(9));

        if (field == 's' && blink)
        {
            clcd_print("  ", LINE2(10));
        }
        else
        {
            clcd_putch('0' + time_arr[2], LINE2(10));
            clcd_putch('0' + time_arr[3], LINE2(11));
        }
    }

    time_count = (time_arr[0] * 10 + time_arr[1]) * 60 + (time_arr[2] * 10 + time_arr[3]);
    operation_mode_flag = 0;
    operation_mode();
}

void operation_mode()
{
    unsigned char key, pause_flag = 0, menu_flag = 0;

    clear_display();
    TMR2ON = 1;
    clcd_print("TIME = ", LINE1(0));

    while (time_count > 0)
    {
        unsigned int minutes = time_count / 60;
        unsigned int seconds = time_count % 60;

        time_arr[0] = minutes / 10;
        time_arr[1] = minutes % 10;
        time_arr[2] = seconds / 10;
        time_arr[3] = seconds % 10;

        clcd_putch('0' + time_arr[0], LINE1(7));
        clcd_putch('0' + time_arr[1], LINE1(8));
        clcd_putch(':', LINE1(9));
        clcd_putch('0' + time_arr[2], LINE1(10));
        clcd_putch('0' + time_arr[3], LINE1(11));

        clcd_print("4.RES", LINE2(0));
        clcd_print("5.PAS", LINE2(6));
        clcd_print("6.SP", LINE2(12));

        if (!pause_flag) RC2 = 1;

        key = read_matrix_keypad(STATE);

        if (key == 4)
        {
            if (!pause_flag)
                time_count += 30;
            else
            {
                TMR2ON = 1;
                RC2 = 1;
                pause_flag = 0;
            }
        }
        else if (key == 5)
        {
            TMR2ON = 0;
            RC2 = 0;
            pause_flag = 1;
        }
        else if (key == 6)
        {
            menu_flag = 1;
            break;
        }
    }

    RC2 = 0;

    if (!menu_flag)
    {
        clear_display();
        clcd_print("TIME UP !!!", LINE2(3));
        clcd_print(" Enjoy your meal", LINE3(0));
        RC1 = 1;
        __delay_ms(500);
        RC1 = 0;
    }

    clear_display();
    menu_screen(RESET_DISPLAY);
}

void convention_mode(unsigned char reset_flag)
{
    if (reset_flag == RESET_DISPLAY)
    {
        clear_display();
        set_temp();
    }
}

void set_temp()
{
    unsigned char preheat_flag = 0, key;
    static char blink_time = 0, blink = 0;
    int temp_key_count = 2, value;

    memset(temp_arr, 0, sizeof(temp_arr));
    clcd_print("SET Temp <*C>", LINE1(0));
    clcd_print("TEMP = ", LINE2(0));
    clcd_print("*:CLEAR  #:ENTER", LINE4(0));

    while (!preheat_flag)
    {
        key = read_matrix_keypad(STATE);

        if (key == '#')
            preheat_flag = 1;

        else if (key == '*')
        {
            memset(temp_arr, 0, sizeof(temp_arr));
            temp_key_count = 2;
        }
        else if (key != ALL_RELEASED && key <= 9 && temp_key_count >= 0)
        {
            temp_arr[0] = temp_arr[1];
            temp_arr[1] = temp_arr[2];
            temp_arr[2] = key;

            value = temp_arr[0] * 100 + temp_arr[1] * 10 + temp_arr[2];

            if (value > 180)
            {
                temp_arr[0] = 1;
                temp_arr[1] = 8;
                temp_arr[2] = 0;
                temp_key_count = 2;
            }

            temp_key_count--;
        }

        if (++blink_time >= 5)
        {
            blink_time = 0;
            blink = !blink;
        }

        for (int i = 0; i < 3; i++)
        {
            clcd_putch(blink ? ' ' : '0' + temp_arr[i], LINE2(8 + i));
        }
    }

    if (preheat_screen() == SUCCESS)
    {
        set_time_screen();
    }
}

unsigned char preheat_screen()
{
    clear_display();
    clcd_print("Pre-heating", LINE1(0));
    clcd_print("Time left: ", LINE2(0));

    temp_time_count = 60;
    TMR2ON = 1;

    while (temp_time_count)
    {
        clcd_putch('0', LINE2(11));
        clcd_putch('0' + temp_time_count / 10, LINE2(12));
        clcd_putch('0' + temp_time_count % 10, LINE2(13));
        clcd_putch('s', LINE2(14));
    }

    return SUCCESS;
}

void start_mode_screen()
{
    time_count = 30;
    operation_mode();
}

void clear_display()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}
