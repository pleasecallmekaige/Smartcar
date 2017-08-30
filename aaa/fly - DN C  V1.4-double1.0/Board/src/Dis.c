#include "Dis.h"
#include "include.h"

extern uint8 KeyValue1;
extern uint8 KeyValue2;
extern uint8 KeyValue3;

void Dis()
{
    LED_P6x8Str(0,0,"camer.error");
    LED_PrintsignValueF4(75, 0,camer.error );
    
    LED_P6x8Str(0,1,"speed_set");
    LED_PrintsignValueF4(75, 1,motor.speed_set );
    
    LED_P6x8Str(0,2,"speed");
    LED_PrintsignValueF4(75, 2,motor.avg_speed ); 
    
    LED_P6x8Str(0,3,"Car Time");
    LED_PrintsignValueF4(75, 3,cartime );
    
    if(KeyValue1==1)
    {
        LED_P6x8Str(45,6,"servo");
    }
    if(KeyValue2==1)
    {
        LED_P6x8Str(32,7,"start 2s");
    }
//    if(KeyValue3==1)
//    {
//        LED_P6x8Str(45,5,"UFF1");
//        UFF=UFF2;      
//    }
//     else
//     {
//        LED_P6x8Str(45,5,"UFF2");
//        UFF=UFF1;       
//     }
}