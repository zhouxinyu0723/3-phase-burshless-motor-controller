#include "triphase_moto.h"
void initial(void)
{
  ini_crystal();
  ini_pwm();
  ini_input_io();
  ini_timer();
  ini_uart();
}
void ini_crystal(void)
{
#if DCO_F==8
   if (CALBC1_8MHZ==0xFF)
  {											
    while(1);
  } 
  DCOCTL = 0;
  BCSCTL1 = CALBC1_8MHZ;
  DCOCTL = CALDCO_8MHZ;
#endif
#if DCO_F==16
   if (CALBC1_16MHZ==0xFF)
  {											
    while(1);
  } 
  DCOCTL = 0;
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;
#endif
}
void ini_pwm(void)
{
  /*
  TIMER_1��ʱ������
  */
  /*
  ѡ��SMCLKΪʱ��Դ
  */
  TA1CTL=TASSEL_2;  
  /*
  TIMERA_1���ó����PWM
  */
  TA1CCTL2=TA1CCTL1=TA1CCTL0=OUTMOD_2;
  /*
  TIMER_1��PWM���ں���������
  */
  TA1CCR0=PWMT_START;
  TA1CCR2=TA1CCR1=PWMH_START;
  /*
  ����TIMER_1
  */
  TA1CTL|=MC_3;
  /*
  IO������
  */
  P2OUT=0x00;
  P2DIR=0xFF;
  P2OUT=0x00;
}
void ini_input_io(void)
{
  P1DIR&=~aainput;P1DIR&=~bbinput;P1DIR&=~ccinput;
  P1OUT|=allinput;
  P1REN|=allinput;
  P1IFG&=~aainput;P1IFG&=~bbinput;P1IFG&=~ccinput;
}
void ini_timer(void)
{
  /*
  ѡ��SMCLKΪʱ��Դ,8��Ƶ
  */
  TA0CTL=TASSEL_2+ID_3;
  /*
  ����TIMER_0
  */
  TA0CTL|=MC_2;
}