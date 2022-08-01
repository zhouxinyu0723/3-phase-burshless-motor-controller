#include "triphase_moto.h"
phase_switcher switcher;
TAV_controler controler;
Uart uart;
int code=0;
int flag=0;
 void start()
 {
  
}
int main( void )
{

  WDTCTL = WDTPW + WDTHOLD;
  __bis_SR_register(GIE);
  initial();
  /*
  Æô¶¯µç»ú
  */
  
  while(1)
  {
    if(code=='T')
    {
      {
        int timer=SPT_START;
        switcher.FP();
        while(timer>SPT_MID)
        {
          wait_100us(timer/DIS_SPSI_TIMES);
          switcher.enable_SPSI();
          wait_100us(timer-timer/DIS_SPSI_TIMES);
          switcher.disable_SPSI();
          switcher.SP();
          timer-=timer/SHRINK_RATE_1+1;
        }
        while(timer>SPT_END)
        {
          wait_100us(timer/DIS_SPSI_TIMES);
          switcher.enable_SPSI();
          wait_100us(timer-timer/DIS_SPSI_TIMES);
          switcher.disable_SPSI();
          switcher.SP();
          timer-=timer/SHRINK_RATE_2+1;
        }
        wait_100us(timer/DIS_SPSI_TIMES);    
        }
      controler.work_state=1;
      switcher.enable_SPSI();
      {
        int i=PWM_START_RATE;
        while(i<80)
        {
          wait_100us(100);
          TA1CCR2=TA1CCR1=i;
          i=i+10;
        }
      }
      code=0;
    }
    if(code=='F')
    {
      switcher.CP();
      switcher.disable_SPSI();
      controler.diable_time_interrupt();
      switcher.CP();
      code=0;
    }
    if(code=='U')
    {
      if(TA1CCR1<=500)
      TA1CCR2=TA1CCR1=TA1CCR1+10;
      code=0;
    }
    if(code=='D')
    {
      if(TA1CCR1>=50)
      TA1CCR2=TA1CCR1=TA1CCR1-10;
      code=0;
    }
    if(code=='G')
    {
      int temp=controler.read_SPT();
      char temp_=temp-900;
      uart.send(temp_,2);
      code=0;
    }
  }
  return 0;
}