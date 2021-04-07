
#include "mbed.h"
#include "mbed_events.h"
using namespace std::chrono;
#include <thread>
#include "uLCD_4DGL.h"
EventQueue queue(32 * EVENTS_EVENT_SIZE);
InterruptIn  pin1(D5);
InterruptIn  pin2(D6);
InterruptIn  pin3(D10);
uLCD_4DGL uLCD(D1, D0, D2);
AnalogOut  aout(PA_4);

int sample = 240;
int g;
float ADCdata[245];
AnalogIn Ain(A0);
Thread thread;
void thread_out(){

    for (g = 0; g < sample; g++){
        printf("%f\r\n", ADCdata[g]);
        ThisThread::sleep_for(240ms/sample);
    }
}
void thread_sample()
{
    for (g = 0; g < sample; g++){
        ADCdata[g] = Ain;
        ThisThread::sleep_for(240ms/sample);
    }
}


float sel_tmp=0.125;
float sel=0;
int mod=0;
void up()
{
    if(sel_tmp!=1)
    {
        mod=0;
        sel_tmp=sel_tmp*2;
    }
}
void down()
{
    if(sel_tmp!=0.125)
    {
        mod=0;
        sel_tmp=sel_tmp/2;
    }
}
void sele()
{
    sel=sel_tmp;
    mod=1;
}
int main(void)
{   
    EventQueue queue;
    thread.start(callback(&queue, &EventQueue::dispatch_forever));
    pin3.rise(queue.event(thread_sample));
    pin3.rise(queue.event(thread_out));
    pin1.rise(&up);
    pin2.rise(&down);
    pin3.rise(&sele);

    while(1)
    {
        g=0;
            uLCD.printf("%f\n",sel_tmp);
            ThisThread::sleep_for(1000ms);
        
        if(mod==1)
        {
        for (float i = 0.0; i < 1.0; i += (0.0125/sel)) {
            aout = i;
            ADCdata[g] = Ain;
            g=g+1;
            ThisThread::sleep_for(1ms);
      }
        for (int t=0;t<80/sel;t=t+1){
            aout =1;
            ADCdata[g] = Ain;
            g=g+1;
            ThisThread::sleep_for(1ms);
        }
        for (float i = 1.0; i > 0; i -= (0.0125/sel)) {
            aout = i;
            ADCdata[g] = Ain;
            g=g+1;
            ThisThread::sleep_for(1ms);
      }
        for(int t=0;t<240;t++)
        printf("%f\r\n", ADCdata[t]);
        }
        uLCD.cls();
    }
}