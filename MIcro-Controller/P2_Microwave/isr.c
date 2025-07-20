#include <xc.h>
#include "main.h"
extern unsigned char time_count;
extern unsigned char temp_time_count;
void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        if (++count == 600)
        {
            count = 0;
            
            if(time_count>0)
            {
                time_count--;
            }
            if(temp_time_count>0)
            {
                temp_time_count--;
            }
        }
        
        TMR2IF = 0;
    }
}