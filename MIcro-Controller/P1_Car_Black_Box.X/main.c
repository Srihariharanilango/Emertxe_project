/*
 
 NAME: Srihariharan Ilango 
 * ID: 24031_016
 * Description: CAR BLACK BOX 
 * 
 * Project: Car Black Box
Abstract:
By considering todays busy life, every one wants to reach the destination as soon as
possible by ignoring the rules and regulations. So one solution could be by implementing a
Black Box which would log critical activities on the car.
As mentioned above the root cause of the negligence in the driving would be meeting the
daily schedule and go off duty, or to earn extra money by illegal trips etc,. So buy
implementing the mentioned solution it would be easy to keep track of how the vehicle is
being used, handled and control the efficiency of the vehicle.
The proposed solution is to log all the critical events like the gear shifts with current speed,
the engine temperature, fuel consumption per trip, trip distance etc., The system should
allow a password based access to the transport managers to view or download the log to PC
if required.
 * 
 * DATE: 08/05/2025
 
 * 
 * NOTE:
 * !!!!!!KINDLY!!!!!!!!!!
 * IN long press select function wait atleast 5 seconds to transfer 
 * 
 
 
 
 */


#include "main.h"
#pragma config WDTE = OFF



void init_config()
{
    //initialize clcd
    init_clcd();
    
    //init adc
    init_adc();
    
    //initialize digital keypad
    init_digital_keypad();
    
    //initialize i2c
    init_i2c(100000);
    
    //initialize rtc
    init_ds1307();
    
    //initialize the timer
    init_timer2();
    init_uart(9600);
    
    puts("CAR BLACK BOX LOGS\n\r");
    PEIE = 1;
    GIE=1;
}
void main(void) {
    init_config();
    unsigned char control_flag = DASHBOARD_SCREEN; //to know about the current name
    unsigned char event[3] ="ON";
    unsigned char speed = 0;
    int wait_time = 0;
    unsigned char *gear[] = {"GN","GR","G1","G2","G3","G4"};
                            //0     1    2   3     4    5
    unsigned char gr=0;//to point the gear index variable
    unsigned char key;
    unsigned int menu_delay = 0;
    int long_press = 0;
    unsigned char reset_flag,menu_pos; //to find how many times it gone to the passwd prompt
    ext_eeprom_24C02_str_write(0x00,"1010");
    speed = read_adc()/10.3;
    //log_event(event,speed);
    while(1){
        speed = read_adc()/10.3; //adc is 10bit resolution so change to 99 we use this 
        //log_event(event,speed);
         __delay_ms(100);
        key = read_digital_keypad(STATE);
       
        
        for(unsigned int i=300;i--;);//delay to avoid bounching effect
        
        if(key==SW1){
            //for collision
            strcpy(event,"CO");
            log_event(event,speed);
        }
        
        else if(key==SW2 &&gr<5){
            //increment the gear
            gr++;
            strcpy(event,gear[gr]);
            log_event(event,speed);

        }
        else if(key==SW3 && gr>0){
            //decrement the gear
            gr--;
            strcpy(event,gear[gr]);
            log_event(event,speed);
            
        }
        else if((key==SW4 || key == SW5)&& control_flag==DASHBOARD_SCREEN){
            
            control_flag = LOGIN_SCREEN;
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            __delay_us(500);
            clcd_print("ENTER PASSWORD",LINE1(1));
            clcd_write(LINE2(4),INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            reset_flag = RESET_PASSWD;
            /* Switching off the Timer2 */
            TMR2ON = 1;
            
        }
   
       else if (control_flag == MAIN_MENU) {
    
        key = read_digital_keypad(LEVEL);

   
}
 
        if(gr>5){
            gr=5;
        }
        
        if(gr<0){
            gr=0;
        }      
        
        
        switch(control_flag){
            
            
            case DASHBOARD_SCREEN:
                display_dashboard(event, speed);
                break;
                
                
            case LOGIN_SCREEN:
                switch(login(key,reset_flag)){
                    case RETURN_BACK:
                        control_flag = DASHBOARD_SCREEN;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag = MAIN_MENU;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        reset_flag = RESET_MENU;
                        continue;//to make reset flag to be reset menu
                    
                }
                reset_flag = RESET_NOTHING;
                break;    
                
               case MAIN_MENU: {
                   
                    TMR2ON = 1;

                     // Show menu and get position (scrolling, not selection)
                    menu_pos = menu_screen(key, reset_flag,&control_flag);
                    switch(menu_pos){
                        case RETURN_BACK: {
                        control_flag = DASHBOARD_SCREEN;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    }
                    
                    }
                    
                    
                    
                    break;
                    
                    }
               
               
                case VIEW_LOG_SCREEN:
                    TMR2ON = 1;
                    clear_display();
                    control_flag = view_log();
                    __delay_ms(200);
                    TMR2ON = 0;
                    break;
              case CLEAR_LOG:{
                  TMR2ON = 1;
                    clear_display();
                  control_flag = clear_log();
                  __delay_ms(200);
                  TMR2ON = 0;
                  break;
              }      
            case DOWNLOAD_LOG:
                TMR2ON =1;
                 clear_display();
                control_flag = download_log();
                clear_display();
                clcd_print("LOGS DOWNLOADED",LINE1(0));
                __delay_ms(1500);
                clear_display();
                TMR2ON =0;
                break;
                
            case CHANGE_PASSWD:
                    
                
                //reset_flag = RESET_PASSWD;
                /* Switching off the Timer2 */
                TMR2ON = 1;
                switch(change_passwd()){
                    case RETURN_BACK:
                        control_flag = MAIN_MENU;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case CHANGE_SUCCESS:
                        control_flag = DASHBOARD_SCREEN;
                        clear_display();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        reset_flag = RESET_MENU;
                        continue;//to make reset flag to be reset menu
                    
                }
                reset_flag = RESET_NOTHING;
                break;
                
            case CHANGE_TIME:
                set_time();
                control_flag = MAIN_MENU;
                reset_flag = RESET_MENU;
                break;
                
                
            
                    }
                    reset_flag = RESET_NOTHING;
                    
                    
    }
    return;
}
