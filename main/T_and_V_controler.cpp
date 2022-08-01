#include "triphase_moto.h"
TAV_controler::TAV_controler(void):pointer(0),work_state(0),pure_control(0),pure_control_times(0){};
void TAV_controler::record_SPT(void)
{
  int now_TA0=read_TA0();
  int now_SPT;
  if(now_TA0>last_TA0)
    now_SPT=now_TA0-last_TA0;
  else
    now_SPT=last_TA0-now_TA0;
  pointer=round_count(pointer,RECORD_NUMBER);
  SPT_record[pointer]=now_SPT;
  last_TA0=now_TA0;
  predict_SPT();
}
void TAV_controler::predict_SPT(void)
{
  unsigned int temp[RECORD_NUMBER];
  for(int i=0;i<RECORD_NUMBER;i++)
  {
    temp[i]=SPT_record[i];
  }
  sort(temp,temp+RECORD_NUMBER);
  SPT=temp[RECORD_NUMBER/2];
}
void TAV_controler::set_time_to(void)
{
  int now_TA0=read_TA0();
  if(work_state==1)
    TA0CCR0=SPT*3+now_TA0;
  if(work_state==2)
    TA0CCR0=SPT/2+now_TA0;
  if(work_state==3)
    TA0CCR0=SPT/3+now_TA0;
  TA0CCTL0|=CCIE;
}
void TAV_controler::diable_time_interrupt(void)
{
  TA0CCTL0&=~CCIE;
}
int TAV_controler::read_TA0(void)const
{
  int temp;
  TA0CTL&=~MC_2;
  temp=TA0R;
  TA0CTL|=MC_2;
  return temp;
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
  controler.diable_time_interrupt();
  if(controler.work_state==1)
  {
    switcher.disable_SPSI();
    switcher.CP();
  }else{
    if(controler.work_state==2)
    {
      controler.work_state=3;
      if(controler.pure_control)
      {
        switcher.SP_false();
        controler.pure_control_times++;
        if(controler.pure_control_times>2)
          controler.pure_control=0;
      }else{
      switcher.SP();
      }
      controler.set_time_to();
    }else{
        controler.work_state=1;
        switcher.enable_SPSI();
        controler.set_time_to();
    }
  }
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
  switcher.disable_SPSI();
  if(!controler.work_state)
  {
    controler.record_SPT();
  }else{
    controler.diable_time_interrupt();
    controler.work_state=2;
    controler.record_SPT();
    controler.set_time_to();
  }
  P1IFG&=~aainput;
  P1IFG&=~bbinput;
  P1IFG&=~ccinput;
}