
#include "mbed.h"
#include "mbed_events.h"
using namespace std::chrono;
#include <thread>
#include "uLCD_4DGL.h"
InterruptIn  pin1(D5);
InterruptIn  pin2(D6);
InterruptIn  pin3(D10);
uLCD_4DGL uLCD(D1, D0, D2);
float sel_tmp=0.125;
float sel=0;
int mod=0;
void up()
{
    if(sel_tmp!=1&&mod==0)
    sel_tmp=sel_tmp*2;
}
void down()
{
    if(sel_tmp!=0.125&&mod==0)
    sel_tmp=sel_tmp/2;
}
void sele()
{
    sel=sel_tmp;
    mod=1;
}
int main(void)
{
    pin1.rise(&up);
    pin2.rise(&down);
    pin3.rise(&sele);
    while(1)
    {
        if(mod==0)
        {
            uLCD.printf("%f\n",sel_tmp);
            ThisThread::sleep_for(500ms);
            uLCD.cls();
        }
        else if(mod==1)
            uLCD.printf("%f\n",sel);
            ThisThread::sleep_for(500ms);
            uLCD.cls();
    }
}