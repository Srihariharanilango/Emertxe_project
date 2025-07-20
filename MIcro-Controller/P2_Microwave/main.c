#include "main.h"

#pragma config WDTE = OFF
#define _XTAL_FREQ 20000000

unsigned char control_flag;

void init_config()
{
    // Initialize system peripherals
    init_clcd();
    init_timer2();
    init_matrix_keypad();

    // Setup output pins
    TRISC1 = 0;  // Buzzer
    TRISC2 = 0;  // Fan

    // Enable interrupts
    PEIE = 1;
    GIE = 1;
}

void main(void)
{
    init_config();

    unsigned char key, reset_flag = RESET_DISPLAY;

    // Splash screen
    clcd_print("Powering on", LINE2(3));
    clcd_print("Microwave oven", LINE3(2));

    for (unsigned char i = 0; i < 16; ++i)
    {
        clcd_putch(0xFF, LINE1(i));
        clcd_putch(0xFF, LINE4(i));
        __delay_ms(10);
    }

    __delay_ms(500);

    control_flag = MENU_SCREEN;

    while (1)
    {
        key = read_matrix_keypad(STATE);

        // Check key inputs only in menu screen
        if (control_flag == MENU_SCREEN)
        {
            switch (key)
            {
                case 1:
                    control_flag = MICRO_MODE_SCREEN;
                    reset_flag = RESET_DISPLAY;
                    break;

                case 2:
                    control_flag = GRILL_MODE_SCREEN;
                    reset_flag = RESET_DISPLAY;
                    break;

                case 3:
                    control_flag = CONVENTION_MODE_SCREEN;
                    reset_flag = RESET_DISPLAY;
                    break;

                case 4:
                    control_flag = START_MODE_SCREEN;
                    break;

                default:
                    break;
            }
        }

        // Handle current screen based on control flag
        switch (control_flag)
        {
            case MENU_SCREEN:
                menu_screen(reset_flag);
                break;

            case MICRO_MODE_SCREEN:
                micro_mode(reset_flag);
                break;

            case GRILL_MODE_SCREEN:
                micro_mode(reset_flag);
                break;

            case CONVENTION_MODE_SCREEN:
                convention_mode(reset_flag);
                break;

            case START_MODE_SCREEN:
                start_mode_screen();
                break;

            default:
                break;
        }

        reset_flag = RESET_NOTHING;
    }
}
